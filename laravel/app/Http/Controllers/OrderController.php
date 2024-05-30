<?php

namespace App\Http\Controllers;

use App\Models\Order;
use App\Models\Product;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Symfony\Component\HttpFoundation\Response;
use Spatie\QueryBuilder\QueryBuilder;

class OrderController extends Controller
{
    public function index()
    {
        $userId = Auth::id();
        $userRole = Auth::user()->role;
        
        $query = QueryBuilder::for(Order::class)
            ->allowedFilters('status')
            ->allowedSorts('id', 'user_id', 'product_id', 'quantity', 'price', 'order_date', 'status', 'created_at', 'updated_at')
            ->defaultSort('id')
            ->with(['product']); // Eager load product and seller with only their names

        if ($userRole === 'admin') {
            $orders = $query->paginate(10);
        } else if ($userRole === 'buyer') {
            $orders = $query->where('user_id', $userId)->paginate(10);
        } else {
            $orders = $query->where('seller_id', $userId)->paginate(10);
        }

        // Transform orders to include product and seller names
        $orders->getCollection()->transform(function ($order) {
            return [
                'product_name' => $order->product->name ?? null,
                'seller_name' => $order->product->seller->first_name ?? null,
                'quantity' => $order->quantity,
                'price' => $order->price,
                'order_date' => $order->order_date,
                'status' => $order->status,
                'created_at' => $order->created_at,
                'updated_at' => $order->updated_at,
            ];
        });

        return response()->json(['success' => true, 'data' => $orders]);
    }

    public function store(Request $request)
    {
        $userId = Auth::id();

        $request->validate([
            'product_id' => 'required|exists:products,id',
            'quantity' => 'nullable|integer',
            'order_date' => 'nullable|date',
            'status' => 'nullable|in:pending,in-progress,completed',
            'seller_id' => 'required|exists:users,id',
        ]);

        $product = Product::findOrFail($request->input('product_id'));
        $totalPrice = $product->price * $request->input('quantity');

        $order = Order::create([
            'user_id' => $userId,
            'product_id' => $request->input('product_id'),
            'seller_id' => $product->seller_id,
            'quantity' => $request->input('quantity'),
            'price' => $totalPrice,
            'order_date' => $request->input('order_date'),
            'status' => $request->input('status'),
        ]);

        return response()->json(['success' => true, 'data' => $order]);
    }

    public function show(Order $order)
    {
        $userId = Auth::id();
        $userRole = Auth::user()->role;

        if ($userRole !== 'admin' && $order->user_id !== $userId) {
            return response()->json(['error' => 'You are not authorized to view this order.'], 403);
        }

        return response()->json(['success' => true, 'data' => $order]);
    }

    public function update(Request $request, Order $order)
    {
        $request->validate([
            'product_id' => 'exists:products,id',
            'quantity' => 'integer',
            'order_date' => 'date',
            'status' => 'in:pending,in-progress,completed',
        ]);

        $userId = Auth::id();
        if ($userId !== $order->user_id && Auth::user()->role !== 'admin') {
            return response()->json(['error' => 'You are not authorized to update this order.'], 403);
        }

        $order->update($request->all());
        $product = $order->product;
        $newTotalPrice = $product->price * $order->quantity;
        $order->update(['price' => $newTotalPrice]);

        return response()->json(['success' => true, 'data' => $order]);
    }

    public function updateStatus(Request $request, Order $order)
    {
        $request->validate([
            'status' => 'required|in:pending,in-progress,completed',
        ]);

        $userId = Auth::id();
        $userRole = Auth::user()->role;

        if ($userRole === 'buyer' && $order->user_id !== $userId) {
            return response()->json(['error' => 'You are not authorized to update this order status.'], 403);
        }

        if ($userRole === 'seller' && $order->seller_id !== $userId) {
            return response()->json(['error' => 'You are not authorized to update this order status.'], 403);
        }

        $order->status = $request->input('status');
        $order->save();

        return response()->json(['success' => true, 'data' => $order]);
    }

    public function destroy(Order $order)
    {
        $userId = Auth::id();
        $userRole = Auth::user()->role;

        if ($userRole !== 'admin' && $order->user_id !== $userId) {
            return response()->json(['error' => 'You are not authorized to delete this order.'], 403);
        }

        $order->delete();
        return response()->json(['success' => true, 'data' => $order], Response::HTTP_NO_CONTENT);
    }
}
