<?php

namespace App\Http\Controllers;

use App\Models\Product;
use Illuminate\Support\Facades\Auth;
use Illuminate\Http\Request;
use Symfony\Component\HttpFoundation\Response;
use Spatie\QueryBuilder\QueryBuilder;

class ProductController extends Controller
{
    public function index()
    {
        $user = Auth::user();

        if ($user->role === 'admin' || $user->role === 'buyer') {
            $products = QueryBuilder::for(Product::class)
                ->allowedFilters('type')
                ->allowedSorts('id', 'name', 'location', 'price', 'quantity', 'type', 'created_at', 'updated_at')
                ->defaultSort('id')
                ->paginate(10);
        } else {
            $products = QueryBuilder::for(Product::class)
                ->where('seller_id', $user->id)
                ->allowedFilters('type')
                ->allowedSorts('id', 'name', 'location', 'price', 'quantity', 'type', 'created_at', 'updated_at')
                ->defaultSort('id')
                ->paginate(10);
        }

        return response()->json(['success' => true, 'data' => $products]);
    }

    public function store(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'name' => 'required|string',
            'description' => 'nullable|string',
            'location' => 'required|string',
            'price' => 'nullable|numeric',
            'quantity' => 'nullable|integer',
            'type' => 'nullable|in:Wood,Diesel',
        ]);

        // Assign the authenticated user's ID as the seller ID
        $request['seller_id'] = $user->id;

        // Create the product
        $product = Product::create($request->all());

        return response()->json(['success' => true, 'data' => $product], 201);
    }

    public function update(Request $request, Product $product)
    {
        $user = Auth::user();

        $request->validate([
            'name' => 'string',
            'description' => 'string',
            'location' => 'string',
            'price' => 'numeric',
            'quantity' => 'integer',
            'type' => 'in:Wood,Diesel',
        ]);

        $request['seller_id'] = $user->id;

        $product->update($request->all());
        return response(['success' => true, 'data' => $product]);
    }

    public function show(Product $product)
    {
        $user = Auth::user();

        if ($user->id !== $product->seller_id || $user->role !== 'admin') {
            return response()->json(['error' => 'You are not authorized to view this product.'], 403);
        }

        return response(['success' => true, 'data' => $product]);
    }

    public function destroy(Product $product)
    {
        $user = Auth::user();

        if ($user->id !== $product->seller_id || $user->role !== 'admin') {
            return response()->json(['error' => 'You are not authorized to delete this product.'], 403);
        }

        $product->delete();
        return response(['data' => $product], Response::HTTP_NO_CONTENT);
    }
}

