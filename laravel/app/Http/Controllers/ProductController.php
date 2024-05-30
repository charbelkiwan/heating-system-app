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

        $products = QueryBuilder::for(Product::class)
            ->allowedFilters('type')
            ->allowedSorts('id', 'name', 'location', 'price', 'quantity', 'type', 'created_at', 'updated_at')
            ->defaultSort('id')
            ->paginate(10);

        return response()->json(['success' => true, 'data' => $products]);
    }

    public function store(Request $request)
    {
        $user = Auth::user();

        if ($user->role !== 'seller') {
            return response()->json(['error' => 'Unauthorized'], 403);
        }

        $request->validate([
            'name' => 'required|string',
            'description' => 'nullable|string',
            'location' => 'required|string',
            'price' => 'nullable|numeric',
            'quantity' => 'nullable|integer',
            'type' => 'nullable|in:Wood,Diesel',
        ]);

        $request['seller_id'] = $user->id;

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

    public function getNearbyProducts(Request $request)
    {
        // Assuming the user is authenticated and we can get their ID from the request
        $user = $request->user();

        // Fetch the user's location
        $userLocation = $user->location;

        // Fetch products matching the user's location
        $products = Product::where('location', $userLocation)->paginate(10); // Adjust pagination as needed

        return response()->json([
            'success' => true,
            'data' => $products
        ]);
    }

    // app/Http/Controllers/ProductController.php

    public function getProductsByType(Request $request)
    {
        $type = $request->query('type');
        $products = Product::where('type', $type)->paginate(10);

        return response()->json([
            'success' => true,
            'data' => $products,
        ]);
    }


}

