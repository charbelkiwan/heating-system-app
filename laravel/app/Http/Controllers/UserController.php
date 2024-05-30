<?php

namespace App\Http\Controllers;

use App\Models\User;
use Illuminate\Http\Request;
use Spatie\QueryBuilder\QueryBuilder;
use Illuminate\Support\Facades\Hash;

class UserController extends Controller
{
    public function index()
    {
        $users = QueryBuilder::for(User::class)
            ->allowedFilters('role')
            ->allowedSorts('id', 'first_name', 'last_name', 'email', 'role')
            ->defaultSort('id')
            ->paginate(10);

        return response()->json(['success' => true, 'data' => $users]);
    }

    public function store(Request $request)
    {
        //\Log::info('Incoming request data:', $request->all()); // Add this line

        $request->validate([
            'first_name' => 'required|string',
            'last_name' => 'required|string',
            'email' => 'required|email|unique:users,email',
            'password' => 'required|string|min:6',
            'role' => 'required|in:seller,buyer',
            'location' => 'required|string',
        ]);

        $user = User::create([
            'first_name' => $request->input('first_name'),
            'last_name' => $request->input('last_name'),
            'email' => $request->input('email'),
            'password' => Hash::make($request->input('password')),
            'role' => $request->input('role'),
            'location' => $request->input('location'),
        ]);

        $user->assignRole($request->input('role'));

        return response()->json($user, 201);
    }


    public function show(User $user)
    {
        return response()->json($user);
    }

    public function update(Request $request, User $user)
    {
        $request->validate([
            'first_name' => 'string',
            'last_name' => 'string',
            'email' => 'email|unique:users,email,' . $user->id,
            'password' => 'string|min:6',
            'role' => 'in:seller,buyer', // Adjust role options as needed
            'location' => 'string', // Add validation for location
        ]);

        $user->update([
            'first_name' => $request->input('first_name', $user->first_name),
            'last_name' => $request->input('last_name', $user->last_name),
            'email' => $request->input('email', $user->email),
            'password' => $request->filled('password') ? Hash::make($request->input('password')) : $user->password,
            'role' => $request->input('role', $user->role),
            'location' => $request->input('location', $user->location), // Add this line
        ]);

        // Update role if provided
        if ($request->filled('role')) {
            $user->syncRoles([$request->input('role')]);
        }

        return response()->json($user);
    }

    public function destroy(User $user)
    {
        $user->delete();
        return response()->json(null, 204);
    }
}


