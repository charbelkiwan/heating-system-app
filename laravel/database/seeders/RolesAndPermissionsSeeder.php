<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use Spatie\Permission\Models\Permission;
use Spatie\Permission\Models\Role;

class RolesAndPermissionsSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $adminRole = Role::create(['name' => 'admin']);
        $sellerRole = Role::create(['name' => 'seller']);
        $buyerRole = Role::create(['name' => 'buyer']);
        
        // Create permissions
        $manageUsers = Permission::create(['name' => 'manage users']);
        $manageInfo = Permission::create(['name' => 'manage info']);
        $getInfo = Permission::create(['name' => 'get info']);
        $getOrders = Permission::create(['name' => 'get orders']);
        $manageOrders = Permission::create(['name' => 'manage orders']);
        $getProducts = Permission::create(['name' => 'get products']);
        $manageProducts = Permission::create(['name' => 'manage products']);

        
        // Assign permissions to roles
        $adminRole->givePermissionTo($manageUsers, $getInfo, $getOrders, $getProducts);
        $sellerRole->givePermissionTo($manageInfo, $manageProducts, $manageOrders);
        $buyerRole->givePermissionTo($manageOrders, $manageInfo, $manageProducts);
    }
}
