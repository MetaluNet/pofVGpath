/*
 * Copyright (c) 2014 Antoine Rousseau <antoine@metalu.net>
 * BSD Simplified License, see the file "LICENSE.txt" in this distribution.
 * See https://github.com/Ant1r/ofxPof for documentation and updates.
 */
#include "pofVGpath.h"

static t_class *pofpath_class;
static t_symbol *s_clear, *s_close, *s_move, *s_line, *s_curve, *s_arc; 

#define NEXT_FLOAT_ARG(var) if((argc>0) && (argv->a_type == A_FLOAT)) { var = atom_getfloat(argv); argv++; argc--; }

static void pofpath_size(void *x, float width, float height);

static void *pofpath_new(t_symbol *s, int argc, t_atom *argv)
{
    pofVGpath* obj = new pofVGpath(pofpath_class);

    float w=0, h=0;

    NEXT_FLOAT_ARG(w);
    NEXT_FLOAT_ARG(h);
    pofpath_size(obj->pdobj, w, h);
    
    return (void*) (obj->pdobj);
}

static void pofpath_free(void *x)
{
	delete (pofVGpath*)(((PdObject*)x)->parent);
}

static void pofpath_fill(void *x, float r, float g, float b, float a)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->path.setFillColor(ofColor(r*255.0, g*255.0, b*255.0, a*255.0));
}

static void pofpath_stroke(void *x, float r, float g, float b, float a)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->path.setStrokeColor(ofColor(r*255.0, g*255.0, b*255.0, a*255.0));
}

static void pofpath_width(void *x, float width)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->path.setStrokeWidth(width);
}

static void pofpath_filled(void *x, float filled)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->path.setFilled(filled!=0);
}

static void pofpath_res(void *x, float res)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->path.setCurveResolution(res);
	px->path.setCircleResolution(res);
}

/*static void pofpath_mesh(void *x, float mesh)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->doMesh = (mesh != 0);
}*/

static void pofpath_fat(void *x, float f)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->fat = f;
}

static void pofpath_size(void *x, float width, float height)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	if(width < 50) width = 50;
	if(height < 50) height = 50;
	px->size.set(width, height);
	px->allocated = false;
}

static void pofpath_xlate(void *x, float xx, float yy)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->xlate = ofPoint(xx, yy);
}

static void pofpath_depth_correction(void *x, float c)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->depth_correction = c;
}

static void pofpath_scale_correction(void *x, float c)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->scale_correction = c;
}

static void pofpath_segmented(void *x, float s)
{
	pofVGpath* px = (pofVGpath*)(((PdObject*)x)->parent);
	px->segmented = (s != 0);
}

void pofVGpath::setup(void)
{
	s_clear = gensym("clear");
	s_close = gensym("close");
	s_move = gensym("moveTo");
	s_line = gensym("lineTo");
	s_curve = gensym("curveTo");
	s_arc = gensym("arc");
	
	pofpath_class = class_new(gensym("pofVGpath"), (t_newmethod)pofpath_new, (t_method)pofpath_free,
		sizeof(PdObject), 0, A_GIMME, A_NULL);
	
	class_addmethod(pofpath_class, (t_method)tellGUI, s_clear,  A_GIMME, A_NULL);
	class_addmethod(pofpath_class, (t_method)tellGUI, s_close,  A_GIMME, A_NULL);
	class_addmethod(pofpath_class, (t_method)tellGUI, s_move,   A_GIMME, A_NULL);
	class_addmethod(pofpath_class, (t_method)tellGUI, s_line,   A_GIMME, A_NULL);
	class_addmethod(pofpath_class, (t_method)tellGUI, s_curve,  A_GIMME, A_NULL);
	class_addmethod(pofpath_class, (t_method)tellGUI, s_arc,    A_GIMME, A_NULL);
	
	class_addmethod(pofpath_class, (t_method)pofpath_fill, gensym("fillColor"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_stroke, gensym("strokeColor"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_width, gensym("strokeWidth"), A_FLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_filled, gensym("filled"), A_FLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_res, gensym("res"), A_FLOAT, A_NULL);
	//class_addmethod(pofpath_class, (t_method)pofpath_mesh, gensym("mesh"), A_FLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_fat, gensym("fat"), A_FLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_size, gensym("size"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_xlate, gensym("xlate"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_depth_correction, gensym("depth_correction"), A_DEFFLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_scale_correction, gensym("scale_correction"), A_DEFFLOAT, A_NULL);
	class_addmethod(pofpath_class, (t_method)pofpath_segmented, gensym("segmented"), A_DEFFLOAT, A_NULL);

	POF_SETUP(pofpath_class);
}


void pofVGpath::drawPath(ofPoint scale)
{
	ofMatrix4x4 mvp = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) * ofGetCurrentMatrix(OF_MATRIX_PROJECTION);

	canvas.beginPath();
	for (const ofPath::Command& c : path.getCommands()) {
		ofPoint to = (ofPoint(c.to.x, c.to.y, c.to.z + depth_correction) * mvp) * scale;
		switch (c.type) {
			case ofPath::Command::moveTo:
				curveVertices.clear();
				canvas.moveTo(to.x, to.y);
				break;
			case ofPath::Command::lineTo:
				curveVertices.clear();
				canvas.lineTo(to.x, to.y);
				break;
			/* TODO:
			case ofPath::Command::bezierTo:
				curveVertices.clear();
				canvas.bezierTo(c.cp1.x, c.cp1.y, c.cp2.x, c.cp2.y, to.x, to.y);
				break;
			*/
			case ofPath::Command::curveTo:
				curveVertices.push_back(ofPoint(to.x, to.y));
				if (curveVertices.size() == 4){
					float x0 = curveVertices[0].x;
					float y0 = curveVertices[0].y;
					float x1 = curveVertices[1].x;
					float y1 = curveVertices[1].y;
					float x2 = curveVertices[2].x;
					float y2 = curveVertices[2].y;
					float x3 = curveVertices[3].x;
					float y3 = curveVertices[3].y;
			
					float t,t2,t3;
					float x,y;
					int curveResolution = path.getCurveResolution();
			
					for (int i = 0; i < curveResolution; i++){
					
						t 	=  (float)i / (float)(curveResolution-1);
						t2 	= t * t;
						t3 	= t2 * t;
					
						x = 0.5f * ( ( 2.0f * x1 ) +
									( -x0 + x2 ) * t +
									( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
									( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );
	
						y = 0.5f * ( ( 2.0f * y1 ) +
									( -y0 + y2 ) * t +
									( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
									( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );
	
						canvas.lineTo(x, y);
					}
					curveVertices.pop_front();
				}
				break;
			default:
				break;
		}
	}
	canvas.strokePath();
}

void pofVGpath::drawSegment()
{
	float x0 = curveVertices[0].x;
	float y0 = curveVertices[0].y;
	float x1 = curveVertices[1].x;
	float y1 = curveVertices[1].y;
	float x2 = curveVertices[2].x;
	float y2 = curveVertices[2].y;
	float x3 = curveVertices[3].x;
	float y3 = curveVertices[3].y;

	float t,t2,t3;
	float x,y;
	int curveResolution = path.getCurveResolution();

	canvas.beginPath();
	canvas.moveTo(x1, y1);
	for (int i = 0; i < curveResolution; i++){
	
		t 	=  (float)i / (float)(curveResolution-1);
		t2 	= t * t;
		t3 	= t2 * t;
	
		x = 0.5f * ( ( 2.0f * x1 ) +
					( -x0 + x2 ) * t +
					( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
					( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );

		y = 0.5f * ( ( 2.0f * y1 ) +
					( -y0 + y2 ) * t +
					( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
					( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );

		canvas.lineTo(x, y);
	}
	canvas.strokePath();
}

void pofVGpath::drawPathSegmented(ofPoint scale)
{
	ofMatrix4x4 mvp = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) * ofGetCurrentMatrix(OF_MATRIX_PROJECTION);
	bool wasMove = false;
	float strokeWidth = path.getStrokeWidth();
	ofPoint last;
	
	for (const ofPath::Command& c : path.getCommands()) {
		ofPoint to = (ofPoint(c.to.x, c.to.y, c.to.z + depth_correction) * mvp) * scale;
		switch (c.type) {
			case ofPath::Command::moveTo:
				curveVertices.clear();
				canvas.moveTo(to.x, to.y);
				wasMove = true;
				break;
			case ofPath::Command::lineTo:
				curveVertices.clear();

				if(strokeWidth > 0) {
					canvas.beginPath();
					canvas.lineCap(ofxNanoVG::LineCap::BUTT);
					canvas.lineWidth(fat + strokeWidth);
					canvas.strokeColor(path.getStrokeColor());
					canvas.moveTo(last.x, last.y);
					canvas.lineTo(to.x, to.y);
					canvas.strokePath();
				}
				canvas.beginPath();
				canvas.lineCap(ofxNanoVG::LineCap::ROUND);
				canvas.lineWidth(fat);
				canvas.strokeColor(path.getFillColor());
				canvas.moveTo(last.x, last.y);
				canvas.lineTo(to.x, to.y);
				canvas.strokePath();
				break;
			/*
			TODO:
			case ofPath::Command::bezierTo:
				curveVertices.clear();
				canvas.bezierTo(c.cp1.x, c.cp1.y, c.cp2.x, c.cp2.y, to.x, to.y);
				break;
			*/
			case ofPath::Command::curveTo:
				curveVertices.push_back(ofPoint(to.x, to.y));
				if (curveVertices.size() == 4){
					//canvas.lineCap(wasMove ? ofxNanoVG::LineCap::ROUND : ofxNanoVG::LineCap::BUTT);
					if(strokeWidth > 0) {
						canvas.lineCap(ofxNanoVG::LineCap::BUTT);
						canvas.lineWidth(fat + strokeWidth);
						canvas.strokeColor(path.getStrokeColor());
						drawSegment();
					}
					
					canvas.lineCap(ofxNanoVG::LineCap::ROUND);
					canvas.lineWidth(fat);
					canvas.strokeColor(path.getFillColor());
					drawSegment();

					curveVertices.pop_front();
				}
				break;
			default:
				break;
		}
		last = to;
		wasMove = false;
	}
	canvas.strokePath();
}

void pofVGpath::draw()
{
	/*if(doMesh && (currentTexture != NULL)) {
		ofMesh mesh = path.getTessellation();
		ofPoint textureSize(currentTexture->getWidth(), currentTexture->getHeight());
		if(scale.x && scale.y) for(auto & v: mesh.getVertices()){
			mesh.addTexCoord(currentTexture->getCoordFromPoint(textureSize.x/scale.x*v.x + 
					textureSize.x/2 ,textureSize.y/scale.y*v.y + textureSize.y/2 ) );
		}
		else for(auto & v: mesh.getVertices()){
			mesh.addTexCoord(currentTexture->getCoordFromPoint(v.x + textureSize.x/2 ,v.y + textureSize.y/2 ));
		}
		mesh.draw();
	}
	else */
	if(fat == -1) {
		if(!allocated) {
			canvas.allocate(size.x, size.y);
			allocated = true;
		}
		ofPushStyle();
		ofPushView();
		canvas.begin();
		canvas.end();
		ofSetupScreenOrtho();
		ofPopView();
		ofPopStyle();
	}
	else
	if(fat > 0) {
		if(!allocated) {
			canvas.allocate(size.x, size.y);
			allocated = true;
		}
		ofPoint viewportsize = ofPoint(ofGetViewportWidth(), ofGetViewportHeight());
		ofPoint trans = ofPoint(viewportsize.x/2, size.y - viewportsize.y/2) + xlate;
		ofPoint scale = viewportsize * ofPoint(0.5, -0.5) * scale_correction;
		ofPushMatrix();
		canvas.begin();
		canvas.lineCap(ofxNanoVG::LineCap::ROUND);
		canvas.lineJoin(ofxNanoVG::LineCap::ROUND);
		canvas.translate(trans.x, trans.y);
		if(segmented) drawPathSegmented(scale);
		else {
			canvas.lineWidth(fat + path.getStrokeWidth());
			canvas.strokeColor(path.getStrokeColor());
			drawPath(scale);
		
			canvas.lineWidth(fat);
			canvas.strokeColor(path.getFillColor());
			drawPath(scale);
		}
		canvas.end();
		ofPopMatrix();
	}
	else {
		path.draw();
	}
}

void pofVGpath::message(int argc, t_atom *argv)
{
	float X=0, Y=0, Z=0, radiusX=0, radiusY=0, angleBegin=0, angleEnd=0;	

	t_symbol *key = atom_getsymbol(argv); 
	argv++; argc--;
  
	if(key == s_clear) path.clear();
	else if(key == s_close) path.close();
	else if(key == s_move) {
		NEXT_FLOAT_ARG(X);
		NEXT_FLOAT_ARG(Y);
		NEXT_FLOAT_ARG(Z);
		path.moveTo(X, Y, Z);
	}
	else if(key == s_line) {
		NEXT_FLOAT_ARG(X);
		NEXT_FLOAT_ARG(Y);
		NEXT_FLOAT_ARG(Z);
		path.lineTo(X, Y, Z);
	}
	else if(key == s_curve) {
		NEXT_FLOAT_ARG(X);
		NEXT_FLOAT_ARG(Y);
		NEXT_FLOAT_ARG(Z);
		path.curveTo(X, Y, Z);
	}
	else if(key == s_arc) {
		NEXT_FLOAT_ARG(X);
		NEXT_FLOAT_ARG(Y);
		NEXT_FLOAT_ARG(Z);
		NEXT_FLOAT_ARG(radiusX);
		NEXT_FLOAT_ARG(radiusY);
		NEXT_FLOAT_ARG(angleBegin);
		NEXT_FLOAT_ARG(angleEnd);
		if(angleBegin > angleEnd) {
			float tmp = angleEnd;
			angleEnd = angleBegin;
			angleBegin = tmp;
		}
		path.arc(X, Y, Z, radiusX, radiusY, angleBegin, angleEnd);
	}
}  

