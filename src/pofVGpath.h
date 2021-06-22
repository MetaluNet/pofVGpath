/*
 * Copyright (c) 2014 Antoine Rousseau <antoine@metalu.net>
 * BSD Simplified License, see the file "LICENSE.txt" in this distribution.
 * See https://github.com/Ant1r/ofxPof for documentation and updates.
 */
#pragma once

#include "pofBase.h"
#include "ofxNanoVG.h"

class pofVGpath;

class pofVGpath: public pofBase {
	public:
		pofVGpath(t_class *Class):
			pofBase(Class), /*doMesh(false),*/ depth_correction(0), scale_correction(0.5), allocated(false)
		{
		}

		ofPath path;
		ofPoint size;
		//ofPoint last;
		//bool doMesh;
		bool segmented;
		//bool wasMove;
		float fat;
		//float strokeWidth;
		float depth_correction;
		float scale_correction;
		ofxNanoVG::Canvas canvas;
		bool allocated;
		deque<ofPoint> curveVertices;

		void drawPath();
		void drawSegment();
		void drawPathSegmented();
		virtual void draw();
		virtual void message(int  arc, t_atom *argv);
		static void setup(void);
};



