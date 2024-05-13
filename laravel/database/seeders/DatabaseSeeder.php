<?php

namespace Database\Seeders;

use App\Models\User;
use App\Models\Product;
use App\Models\Order;
// use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

class DatabaseSeeder extends Seeder
{
    /**
     * Seed the application's database.
     */
    public function run(): void
    {
        // User::factory(10)->create();

        // User::factory()->create([
        //     'name' => 'Test User',
        //     'email' => 'test@example.com',
        // ]);
        $this->call([RolesAndPermissionsSeeder::class,]);

        $user = User::create([
            'first_name' => 'Admin',
            'last_name' => 'Admin',
            'email' => 'admin@miniproject.com',
            'role' => 'admin',
            'password' => bcrypt('miniproject@2024!'),
            'access_type' => '100',
        ]);

        $user->assignRole('admin');

        User::factory()->count(10)->create();

        // Generate fake products
        Product::factory()->count(10)->create();

        // Generate fake orders
        Order::factory()->count(5)->create();
    }
}
