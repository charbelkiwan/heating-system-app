<?php

namespace App\Http\Controllers;

use App\Models\Order;
use App\Models\Product;
use Illuminate\Http\Request;
use Symfony\Component\HttpFoundation\Response;
use Spatie\QueryBuilder\QueryBuilder;
use Illuminate\Support\Facades\Auth;

class OrderController extends Controller
{
    public function index()
    {
        $userId = Auth::id();
        $userRole = Auth::user()->role;
        
        if ($userRole === 'admin') {
            $orders = QueryBuilder::for(Order::class)
                ->allowedFilters('status')
                ->allowedSorts('id', 'user_id', 'product_id', 'quantity', 'price', 'order_date', 'status', 'created_at', 'updated_at')
                ->defaultSort('id')
                ->paginate(10);
        } else if($userRole === 'buyer'){
            $orders = QueryBuilder::for(Order::class)
                ->allowedFilters(['status'])
                ->allowedSorts(['id', 'user_id', 'product_id', 'quantity', 'price', 'order_date', 'status', 'created_at', 'updated_at'])
                ->where('user_id', $userId)
                ->defaultSort('id')
                ->paginate(10);
        } else{
            $orders = QueryBuilder::for(Order::class)
                ->allowedFilters(['status'])
                ->allowedSorts(['id', 'user_id', 'product_id', 'quantity', 'price', 'order_date', 'status', 'created_at', 'updated_at'])
                ->where('seller_id', $userId)
                ->defaultSort('id')
                ->paginate(10);
        }

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
        ]);

        // Fetch the product to get its price
        $product = Product::findOrFail($request->input('product_id'));

        // Calculate total price based on product price and quantity
        $totalPrice = $product->price * $request->input('quantity');

        // Create the order with the calculated total price
        $order = Order::create([
            'user_id' => $userId,
            'product_id' => $request->input('product_id'),
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

        if ($userRole !== 'admin' || $order->user_id !== $userId) {
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

        // Check if the user is authorized to update the order
        $userId = Auth::id();
        if ($userId !== $order->user_id || Auth::user()->role !== 'admin') {
            return response()->json(['error' => 'You are not authorized to update this order.'], 403);
        }

        // Update the order with the provided data
        $order->update($request->all());

        // Calculate the new total price based on the updated quantity
        $product = $order->product;
        $newTotalPrice = $product->price * $order->quantity;

        // Update the order's price with the new total price
        $order->update(['price' => $newTotalPrice]);

        return response()->json(['success' => true, 'data' => $order]);
    }


    public function destroy(Order $order)
    {
        $userId = Auth::id();
        $userRole = Auth::user()->role;

        // Check if the user is the buyer of the order or is an admin
        if ($userRole !== 'admin' && $order->user_id !== $userId) {
            return response()->json(['error' => 'You are not authorized to delete this order.'], 403);
        }

        $order->delete();
        return response()->json(['success' => true, 'data' => $order], Response::HTTP_NO_CONTENT);
    }
}

