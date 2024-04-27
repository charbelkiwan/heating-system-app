<?php

namespace Database\Factories;

use App\Models\Order;
use App\Models\User;
use App\Models\Product;
use Illuminate\Database\Eloquent\Factories\Factory;

class OrderFactory extends Factory
{
    protected $model = Order::class;

    public function definition()
    {
        $product = Product::inRandomOrder()->first(); // Get a random product
        $quantity = $this->faker->numberBetween(1, 10); // Random quantity

        return [
            'user_id' => User::where('role', 'buyer')->inRandomOrder()->first()->id, // Assign random buyer
            'seller_id' => User::where('role', 'seller')->inRandomOrder()->first()->id, // Assign random seller
            'product_id' => $product->id,
            'quantity' => $quantity,
            'price' => $product->price * $quantity, // Calculate price based on product price and quantity
            'order_date' => $this->faker->dateTimeBetween('-1 year', 'now'), // Random order date
            'status' => $this->faker->randomElement(['pending', 'in-progress', 'completed']),
        ];
    }
}

