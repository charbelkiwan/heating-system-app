<?php

namespace Database\Factories;

use App\Models\Product;
use App\Models\User;
use Illuminate\Database\Eloquent\Factories\Factory;

class ProductFactory extends Factory
{
    protected $model = Product::class;

    public function definition()
    {
        return [
            'name' => $this->faker->sentence,
            'description' => $this->faker->paragraph,
            'location' => $this->faker->address,
            'price' => $this->faker->randomFloat(2, 10, 100), // Random price
            'quantity' => $this->faker->numberBetween(1, 100),
            'type' => $this->faker->randomElement(['Wood', 'Diesel']),
            'seller_id' => User::where('role', 'seller')->inRandomOrder()->first()->id, // Assign random seller
        ];
    }
}

