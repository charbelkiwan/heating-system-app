<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('personal_informations', function (Blueprint $table) {
            $table->id();
            $table->unsignedInteger('title')->nullable();
            $table->string('first_name')->nullable();
            $table->string('last_name')->nullable();
            $table->string('middle_name')->nullable();
            $table->string('display_name')->nullable();
            $table->date('date_of_birth')->nullable();
            $table->unsignedInteger('gender')->nullable();
            $table->string('nationality')->nullable();
            $table->unsignedInteger('marital_status')->nullable();
            $table->string('profile_picture')->nullable();
            $table->text('bio')->nullable();

            $table->foreignId('user_id')->nullable()->constrained('users')->nullOnDelete();
            
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('personal_informations');
    }
};
