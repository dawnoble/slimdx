/*
* Copyright (c) 2007-2008 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#include <d3d9.h>
#include <d3dx9.h>
#include <vcclr.h>

#include "../DataStream.h"
#include "../ComObject.h"
#include "../Math/Math.h"

#include "Direct3D9Exception.h"

#include "AnimationSet.h"
#include "AnimationController.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace SlimDX
{
namespace Direct3D9
{
	AnimationController::AnimationController( ID3DXAnimationController *controller )
	{
		Construct(controller);
	}

	AnimationController::AnimationController( IntPtr pointer )
	{
		Construct( pointer, NativeInterface );
	}

	AnimationController::AnimationController( int maxAnimationOutputs, int maxAnimationSets, int maxTracks, int maxEvents )
	{
		HRESULT hr = D3DXCreateAnimationController( maxAnimationOutputs, maxAnimationSets, maxTracks, maxEvents, 
			reinterpret_cast<LPD3DXANIMATIONCONTROLLER*>(InternalPointer) );
		Result::Record( hr );

		if( FAILED( hr ) )
			throw gcnew Direct3D9Exception();
	}

	Result AnimationController::AdvanceTime( double time, AnimationCallback^ handler )
	{
		LPD3DXANIMATIONCALLBACKHANDLER callback = NULL;
		if(handler != nullptr)
			callback = reinterpret_cast< LPD3DXANIMATIONCALLBACKHANDLER >( Marshal::GetFunctionPointerForDelegate( handler ).ToPointer() );

		HRESULT hr = InternalPointer->AdvanceTime( time, callback );
		return Result::Record( hr );
	}

	AnimationController^ AnimationController::Clone( int maxAnimationOutputs, int maxAnimationSets, int maxTracks, int maxEvents )
	{
		LPD3DXANIMATIONCONTROLLER pointer;

		HRESULT hr = InternalPointer->CloneAnimationController( maxAnimationOutputs, maxAnimationSets, maxTracks, maxEvents, &pointer );

		if( Result::Record( hr ).IsFailure )
			return nullptr;

		return gcnew AnimationController( pointer );
	}

	AnimationSet^ AnimationController::GetAnimationSet( int index )
	{
		LPD3DXANIMATIONSET set;

		HRESULT hr = InternalPointer->GetAnimationSet( index, &set );

		if( Result::Record( hr ).IsFailure )
			return nullptr;

		return gcnew AnimationSet( set );
	}

	AnimationSet^ AnimationController::GetAnimationSet( String^ name )
	{
		LPD3DXANIMATIONSET set;
		array<unsigned char>^ nameBytes = System::Text::ASCIIEncoding::ASCII->GetBytes( name );
		pin_ptr<unsigned char> pinnedName = &nameBytes[0];

		HRESULT hr = InternalPointer->GetAnimationSetByName( reinterpret_cast<LPCSTR>( pinnedName ), &set );
		
		if( Result::Record( hr ).IsFailure )
			return nullptr;

		return gcnew AnimationSet( set );
	}

	int AnimationController::GetCurrentTrackEvent( int track, EventType eventType )
	{
		return InternalPointer->GetCurrentTrackEvent( track, static_cast<D3DXEVENT_TYPE>( eventType ) );
	}

	EventDescription AnimationController::GetEventDescription( int handle )
	{
		EventDescription result;

		HRESULT hr = InternalPointer->GetEventDesc( handle, reinterpret_cast<LPD3DXEVENT_DESC>( &result ) );
		Result::Record( hr );

		return result;
	}

	AnimationSet^ AnimationController::GetTrackAnimationSet( int track )
	{
		LPD3DXANIMATIONSET set;

		HRESULT hr = InternalPointer->GetTrackAnimationSet( track, &set );
		
		if( Result::Record( hr ).IsFailure )
			return nullptr;

		return gcnew AnimationSet( set );
	}

	TrackDescription AnimationController::GetTrackDescription( int track )
	{
		TrackDescription result;

		HRESULT hr = InternalPointer->GetTrackDesc( track, reinterpret_cast<LPD3DXTRACK_DESC>( &result ) );
		Result::Record( hr );

		return result;
	}

	int AnimationController::GetUpcomingPriorityBlend( int handle )
	{
		return InternalPointer->GetUpcomingPriorityBlend( handle );
	}

	int AnimationController::GetUpcomingTrackEvent( int track, int handle )
	{
		return InternalPointer->GetUpcomingTrackEvent( track, handle );
	}

	int AnimationController::KeyPriorityBlend( float newBlendWeight, double startTime, double duration, TransitionType transition )
	{
		return InternalPointer->KeyPriorityBlend( newBlendWeight, startTime, duration, static_cast<D3DXTRANSITION_TYPE>( transition ) );
	}

	int AnimationController::KeyTrackEnable( int track, bool enable, double startTime )
	{
		return InternalPointer->KeyTrackEnable( track, enable, startTime );
	}

	int AnimationController::KeyTrackPosition( int track, double position, double startTime )
	{
		return InternalPointer->KeyTrackPosition( track, position, startTime );
	}

	int AnimationController::KeyTrackSpeed( int track, float newSpeed, double startTime, double duration, TransitionType transition )
	{
		return InternalPointer->KeyTrackSpeed( track, newSpeed, startTime, duration, static_cast<D3DXTRANSITION_TYPE>( transition ) );
	}

	int AnimationController::KeyTrackWeight( int track, float newWeight, double startTime, double duration, TransitionType transition )
	{
		return InternalPointer->KeyTrackWeight( track, newWeight, startTime, duration, static_cast<D3DXTRANSITION_TYPE>( transition ) );
	}

	Result AnimationController::RegisterAnimationOutput( String^ name, AnimationOutput^ output )
	{
		D3DXMATRIX *matrix = NULL;
		D3DXVECTOR3 *scale = NULL;
		D3DXVECTOR3 *translation = NULL;
		D3DXQUATERNION *rotation = NULL;
		array<unsigned char>^ nameBytes = System::Text::ASCIIEncoding::ASCII->GetBytes( name );
		pin_ptr<unsigned char> pinnedName = &nameBytes[0];
		pin_ptr<Matrix> pinMatrix;
		pin_ptr<Vector3> pinScale;
		pin_ptr<Vector3> pinTranslation;
		pin_ptr<Quaternion> pinQuaternion;

		if( (output->Flags & AnimationOutputFlags::Transformation) == AnimationOutputFlags::Transformation )
		{
			pinMatrix = &output->Transformation;
			matrix = reinterpret_cast<D3DXMATRIX*>( pinMatrix );
		}

		if( (output->Flags & AnimationOutputFlags::Scale) == AnimationOutputFlags::Scale )
		{
			pinScale = &output->Scaling;
			scale = reinterpret_cast<D3DXVECTOR3*>( pinScale );
		}

		if( (output->Flags & AnimationOutputFlags::Translation) == AnimationOutputFlags::Translation )
		{
			pinTranslation = &output->Translation;
			translation = reinterpret_cast<D3DXVECTOR3*>( pinTranslation );
		}

		if( (output->Flags & AnimationOutputFlags::Rotation) == AnimationOutputFlags::Rotation )
		{
			pinQuaternion = &output->Rotation;
			rotation = reinterpret_cast<D3DXQUATERNION*>( pinQuaternion );
		}

		HRESULT hr = InternalPointer->RegisterAnimationOutput( reinterpret_cast<LPCSTR>( pinnedName ), matrix, scale, rotation, translation );
		return Result::Record( hr );
	}

	Result AnimationController::RegisterAnimationSet( AnimationSet^ set )
	{
		HRESULT hr = InternalPointer->RegisterAnimationSet( reinterpret_cast<LPD3DXANIMATIONSET>( set->ComPointer.ToPointer() ) );
		return Result::Record( hr );
	}

	Result AnimationController::ResetTime()
	{
		HRESULT hr = InternalPointer->ResetTime();
		return Result::Record( hr );
	}

	Result AnimationController::SetTrackAnimationSet( int track, AnimationSet^ set )
	{
		HRESULT hr = InternalPointer->SetTrackAnimationSet( track, reinterpret_cast<LPD3DXANIMATIONSET>( set->ComPointer.ToPointer() ) );
		return Result::Record( hr );
	}

	Result AnimationController::SetTrackDescription( int track, TrackDescription description )
	{
		HRESULT hr = InternalPointer->SetTrackDesc( track, reinterpret_cast<LPD3DXTRACK_DESC>( &description ) );
		return Result::Record( hr );
	}

	Result AnimationController::EnableTrack( int track )
	{
		HRESULT hr = InternalPointer->SetTrackEnable( track, true );
		return Result::Record( hr );
	}

	Result AnimationController::DisableTrack( int track )
	{
		HRESULT hr = InternalPointer->SetTrackEnable( track, false );
		return Result::Record( hr );
	}

	Result AnimationController::SetTrackPosition( int track, double position )
	{
		HRESULT hr = InternalPointer->SetTrackPosition( track, position );
		return Result::Record( hr );
	}

	Result AnimationController::SetTrackPriority( int track, TrackPriority priority )
	{
		HRESULT hr = InternalPointer->SetTrackPriority( track, static_cast<D3DXPRIORITY_TYPE>( priority ) );
		return Result::Record( hr );
	}

	Result AnimationController::SetTrackSpeed( int track, float speed )
	{
		HRESULT hr = InternalPointer->SetTrackSpeed( track, speed );
		return Result::Record( hr );
	}

	Result AnimationController::SetTrackWeight( int track, float weight )
	{
		HRESULT hr = InternalPointer->SetTrackWeight( track, weight );
		return Result::Record( hr );
	}

	Result AnimationController::UnkeyAllPriorityBlends()
	{
		HRESULT hr = InternalPointer->UnkeyAllPriorityBlends();
		return Result::Record( hr );
	}

	Result AnimationController::UnkeyAllTrackEvents( int track )
	{
		HRESULT hr = InternalPointer->UnkeyAllTrackEvents( track );
		return Result::Record( hr );
	}

	Result AnimationController::UnkeyEvent( int handle )
	{
		HRESULT hr = InternalPointer->UnkeyEvent( handle );
		return Result::Record( hr );
	}

	Result AnimationController::UnregisterAnimationSet( AnimationSet^ set )
	{
		HRESULT hr = InternalPointer->UnregisterAnimationSet( reinterpret_cast<LPD3DXANIMATIONSET>( set->ComPointer.ToPointer() ) );
		return Result::Record( hr );
	}

	bool AnimationController::ValidateEvent( int handle )
	{
		HRESULT hr = InternalPointer->ValidateEvent( handle );

		return (hr == S_OK);
	}

	double AnimationController::Time::get()
	{
		return InternalPointer->GetTime();
	}

	int AnimationController::CurrentPriorityBlend::get()
	{
		return InternalPointer->GetCurrentPriorityBlend();
	}

	float AnimationController::PriorityBlend::get()
	{
		return InternalPointer->GetPriorityBlend();
	}

	void AnimationController::PriorityBlend::set( float value )
	{
		InternalPointer->SetPriorityBlend( value );
	}

	int AnimationController::AnimationSetCount::get()
	{
		return InternalPointer->GetNumAnimationSets();
	}

	int AnimationController::MaxAnimationOutputs::get()
	{
		return InternalPointer->GetMaxNumAnimationOutputs();
	}

	int AnimationController::MaxAnimationSets::get()
	{
		return InternalPointer->GetMaxNumAnimationSets();
	}

	int AnimationController::MaxTracks::get()
	{
		return InternalPointer->GetMaxNumTracks();
	}

	int AnimationController::MaxEvents::get()
	{
		return InternalPointer->GetMaxNumEvents();
	}
}
}
