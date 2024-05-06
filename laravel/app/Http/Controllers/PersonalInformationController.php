<?php

namespace App\Http\Controllers;

use App\Models\PersonalInformation;
use Illuminate\Support\Facades\Auth;
use Illuminate\Http\Request;
use Symfony\Component\HttpFoundation\Response;
use Spatie\QueryBuilder\QueryBuilder;

class PersonalInformationController extends Controller
{
    public function index()
    {
            $personalInformations = QueryBuilder::for(PersonalInformation::class)
                ->allowedFilters('user_id')
                ->allowedSorts('id', 'first_name', 'last_name', 'created_at', 'updated_at')
                ->defaultSort('id')
                ->paginate(10);

        return response()->json(['success' => true, 'data' => $personalInformations]);
    }

    public function store(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'title' => 'nullable|integer',
            'first_name' => 'nullable|string',
            'last_name' => 'nullable|string',
            'middle_name' => 'nullable|string',
            'display_name' => 'nullable|string',
            'date_of_birth' => 'nullable|date',
            'gender' => 'nullable|integer',
            'nationality' => 'nullable|string',
            'marital_status' => 'nullable|integer',
            'profile_picture' => 'nullable|string',
            'bio' => 'nullable|string',
        ]);

        $request['user_id'] = $user->id;

        $personalInformation = PersonalInformation::create($request->all());

        return response()->json(['success' => true, 'data' => $personalInformation], 201);
    }

    public function update(Request $request, PersonalInformation $personalInformation)
    {
        $user = Auth::user();

        $request->validate([
            'title' => 'nullable|integer',
            'first_name' => 'nullable|string',
            'last_name' => 'nullable|string',
            'middle_name' => 'nullable|string',
            'display_name' => 'nullable|string',
            'date_of_birth' => 'nullable|date',
            'gender' => 'nullable|integer',
            'nationality' => 'nullable|string',
            'marital_status' => 'nullable|integer',
            'profile_picture' => 'nullable|string',
            'bio' => 'nullable|string',
        ]);

        if ($user->id !== $personalInformation->user_id) {
            return response()->json(['error' => 'You are not authorized to update this personal information.'], 403);
        }

        $personalInformation->update($request->all());
        return response(['success' => true, 'data' => $personalInformation]);
    }

    public function show(PersonalInformation $personalInformation)
    {
        $user = Auth::user();

        if ($user->role !== 'admin' && $user->id !== $personalInformation->user_id) {
            return response()->json(['error' => 'You are not authorized to view this personal information.'], 403);
        }

        return response(['success' => true, 'data' => $personalInformation]);
    }

    public function destroy(PersonalInformation $personalInformation)
    {
        $user = Auth::user();

        if ($user->role !== 'admin' && $user->id !== $personalInformation->user_id) {
            return response()->json(['error' => 'You are not authorized to delete this personal information.'], 403);
        }

        $personalInformation->delete();
        return response(['data' => $personalInformation], Response::HTTP_NO_CONTENT);
    }
}
