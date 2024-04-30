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
        $manageUsersPermission = Permission::create(['name' => 'manage users']);
        $editProductsPermission = Permission::create(['name' => 'edit products']);
        $manageOrdersPermission = Permission::create(['name' => 'manage orders']);
        $createOrdersPermission = Permission::create(['name' => 'create orders']);
        
        // Assign permissions to roles
        $adminRole->givePermissionTo($manageUsersPermission, $editProductsPermission, $manageOrdersPermission);
        $sellerRole->givePermissionTo($editProductsPermission);
        $buyerRole->givePermissionTo($createOrdersPermission);
    }
}
