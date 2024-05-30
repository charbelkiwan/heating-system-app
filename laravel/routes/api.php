<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\SessionController;
use App\Http\Controllers\UserController;
use App\Http\Controllers\PersonalInformationController;
use App\Http\Controllers\ProductController;
use App\Http\Controllers\OrderController;

// Authenticated routes
Route::middleware('auth:sanctum')->group(function () {
    Route::get('/me', [SessionController::class, 'me']);
    Route::patch('/me', [SessionController::class, 'updateMe']);
    Route::get('/products/nearby', [ProductController::class, 'getNearbyProducts']);
});

// Public routes
Route::post('login', [SessionController::class, 'login'])->middleware(['throttle:10,3']);
Route::post('signup', [SessionController::class, 'signup'])->middleware(['throttle:10,3']);

// Authenticated routes
Route::middleware(['auth:sanctum'])->group(function () {
    Route::post('logout', [SessionController::class, 'logout']);

    // Admin routes
    Route::middleware(['role:admin'])->group(function () {
        Route::apiResource('users', UserController::class);
        Route::apiResource('personal-informations', PersonalInformationController::class)->only(['index', 'show', 'destroy']);
    });

    // Buyer and Seller routes
    Route::middleware(['permission:manage info'])->group(function () {
        Route::apiResource('personal-informations', PersonalInformationController::class)->except(['index']);
    });
});

// Public routes with role-based middleware
Route::middleware(['auth:sanctum', 'permission:manage orders'])->apiResource('orders', OrderController::class);
Route::middleware(['auth:sanctum', 'permission:manage products'])->apiResource('products', ProductController::class);
