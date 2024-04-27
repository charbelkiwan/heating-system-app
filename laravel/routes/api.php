<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\SessionController;
use App\Http\Controllers\UserController;
use App\Http\Controllers\ProductController;
use App\Http\Controllers\OrderController;

Route::post('login', [SessionController::class, 'login'])->middleware(['guest', 'recaptcha', 'throttle:10,3']);
Route::post('signup', [SessionController::class, 'signup'])->middleware(['guest', 'recaptcha', 'throttle:10,3']);

Route::post('logout', [SessionController::class, 'logout'])->middleware('auth:sanctum');

Route::group(['middleware' => ['auth:sanctum', 'role:admin']], function () {
    Route::apiResource('users', UserController::class);
    Route::apiResource('products', ProductController::class);
    Route::apiResource('orders', OrderController::class)->except(['store']);
});

Route::group(['middleware' => ['auth:sanctum', 'role:seller']], function () {
    Route::apiResource('products', ProductController::class);
    Route::get('orders', [OrderController::class, 'index']);
});

Route::group(['middleware' => ['auth:sanctum', 'role:buyer']], function () {
    Route::apiResource('orders', OrderController::class);
    Route::get('products', [ProductController::class, 'index']);
});
