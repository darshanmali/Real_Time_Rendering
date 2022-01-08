
package com.darshan_vilas_mali.multi_color_pyramid_cube;

import android.content.Context;//for grawing context related
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;// embeded GL J2ME(Java to micro Edition farm brought OpenGL on android )
import javax.microedition.khronos.opengles.GL10;// OpenGL 1.0 version J2ME(Java to micro Edition farm brought OpenGL on android )


import android.opengl.GLES32; //for OpenGLES 3.2
import android.view.MotionEvent; // Motion Events
import android.view.GestureDetector;//
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.opengl.Matrix;


public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,OnGestureListener, OnDoubleTapListener{

	private final Context context;

	private GestureDetector gestureDetector;

	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int ShaderProgramObject;

	private int[] Cube_vao = new int[1];
	private int[] vbo_Cube_Vertex = new int[1];
	private int[] vbo_Cube_Color = new int[1];

	private int[] Pyramid_vao = new int[1];
	private int[] vbo_Pyramid_Vertex = new int[1];
	private int[] vbo_Pyramid_Color = new int[1];
	
	private int mvpUniform;
	private float angle_p =0.0f;

	private float perspectivegraphicProjectionMatrix[] = new float[16];

	public GLESView(Context drawingContext)
	{
		super(drawingContext);

		context = drawingContext;

		//NDK chya OpenGL(SERVER) la sangtoy SDK cha OpenGL ki majha version set kar 3.
		setEGLContextClientVersion(3);

		//mich drawing karnar aahe mhanun me sangto this pathun
		setRenderer(this);

		//Render the view only when there is a change in the drawing data. in simple way this is " Invalidate rect "
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		gestureDetector = new GestureDetector(context, this, null, false);
		//context = global envornmental variable  listener =
		gestureDetector.setOnDoubleTapListener(this);

	}

	//GLSurfaceView's Methods
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {


		String glesversion = gl.glGetString(GL10.GL_VERSION);
		System.out.println("DVM :OpenGL-ES Version =  "  + glesversion);

		String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
		System.out.println("DVM :OpenGL-GLSL Version =  "  + glslVersion);


		System.out.println("DVM :Before the Initialize...!!! ");

		initialize(gl);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {

		System.out.println("DVM :Before the resize from OnSurfaceChanged ...!!! ");
		resize(width, height);
		System.out.println("DVM :After the resize from OnSurfaceChanged ...!!! ");
	}

	@Override
	public void onDrawFrame(GL10 gl) {

		display();
	}

	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		int eventaction = event.getAction();//it is not used by use but in future it will be "  "
		if(!gestureDetector.onTouchEvent(event)) //jar majha nasel event tar tya event la baapa la pathav
		{
			super.onTouchEvent(event);//baapachya touch event la call kar
		}
		return true;
	}

	@Override
	public boolean onDoubleTap(MotionEvent e) {
		System.out.println("DVM: Double Tap");
		return true;
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e) {
		return true;
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e) {
		System.out.println("DVM: Single Tap");

		return true;
	}

	@Override
	public boolean onDown(MotionEvent e) {
		return false;
	}

	@Override
	public void onShowPress(MotionEvent e) {

	}

	@Override
	public boolean onSingleTapUp(MotionEvent e) {
		return true;
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
		System.out.println("DVM: Scroll Tap");
		UnInitialized();
		System.exit(0);

		return true;
	}

	@Override
	public void onLongPress(MotionEvent e) {

	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
		return true;
	}

	private void initialize(GL10 gl)
	{
		/* Vertex Shader */
		vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		final String vertexShaderSourceCode = String.format(
				"#version 320 es\n" +
				"in vec4 vPosition;\n" +
				"in vec4 vColor;\n" +
				"out vec4 outColor;\n" +
				"uniform mat4 u_mvp_matrix;\n" +
				"void main(void){\n" +
				"gl_Position = u_mvp_matrix * vPosition;" +
				"outColor = vColor;\n" +
				"}");

		GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);
		GLES32.glCompileShader(vertexShaderObject);

		int [] iShaderCompiledStetus = new int[1];
		int[] iInfoLogLength = new int[1];
		String szInfoLog = null;
		GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_COMPILE_STATUS, iShaderCompiledStetus, 0);
		if(iShaderCompiledStetus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH,iInfoLogLength, 0);
			if(iInfoLogLength[0] > 0)
			{
				szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
				System.out.println("DVM : Vertex Shader Compilation log = " + szInfoLog);
				UnInitialized();
				System.exit(0);
			}
		}


		/* Fragment Shader */
		fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);


		final String fragmentShaderSourceCode = String.format(
				"#version 320 es\n" +
				"precision highp float;" +
				"in vec4 outColor;\n" +
				"out vec4 FragColor;\n" +
				"void main(void){\n" +
				"FragColor = outColor;" +
				"}");

		iShaderCompiledStetus[0] = 0;
		iInfoLogLength[0] = 0;
		szInfoLog = null;
		GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);
		GLES32.glCompileShader(fragmentShaderObject);

		GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_COMPILE_STATUS, iShaderCompiledStetus, 0);
		if(iShaderCompiledStetus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH,iInfoLogLength, 0);
			if(iInfoLogLength[0] > 0)
			{
				szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
				System.out.println("DVM : Fragment Shader Compilation log = " + szInfoLog);
				UnInitialized();
				System.exit(0);
			}
		}

		ShaderProgramObject = GLES32.glCreateProgram();
		GLES32.glAttachShader(ShaderProgramObject, vertexShaderObject);
		GLES32.glAttachShader(ShaderProgramObject, fragmentShaderObject);

		GLES32.glBindAttribLocation(ShaderProgramObject, GLESMacros.DVM_ATTRIBUTE_VERTEX,"vPosition");
		GLES32.glBindAttribLocation(ShaderProgramObject, GLESMacros.DVM_ATTRIBUTE_COLOR,"vColor");

		GLES32.glLinkProgram(ShaderProgramObject);
		int[]iShaderProgramLinkStatus = new int[1];
		iInfoLogLength[0] = 0;
		szInfoLog = null;
		GLES32.glGetProgramiv(ShaderProgramObject,GLES32.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);
		if(iShaderProgramLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(ShaderProgramObject, GLES32.GL_INFO_LOG_LENGTH,iInfoLogLength, 0);
			if(iInfoLogLength[0] > 0)
			{
				szInfoLog = GLES32.glGetShaderInfoLog(ShaderProgramObject);
				System.out.println("DVM : Shader Program Link log = " + szInfoLog);
				UnInitialized();
				System.exit(0);
			}
		}

		mvpUniform = GLES32.glGetUniformLocation( ShaderProgramObject, "u_mvp_matrix");

		/* Pyramid */
		final float PyramidVertices[] = new float[]{

				0.0f, 1.0f, 0.0f,
        		-1.0f, -1.0f, 1.0f,
        		1.0f, -1.0f, 1.0f,
        
        		0.0f, 1.0f, 0.0f,
	        	1.0f, -1.0f, 1.0f,
        		1.0f, -1.0f, -1.0f,
        
        		0.0f, 1.0f, 0.0f,
        		1.0f, -1.0f, -1.0f,
	        	-1.0f, -1.0f, -1.0f,
        
        		0.0f, 1.0f, 0.0f,
    	    	-1.0f, -1.0f, 1.0f,
	        	-1.0f, -1.0f, -1.0f
		};

		//Color
		final float PyramidColor[] = new float[]{
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,

				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,

				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,

				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,

				};

		final float CubeVertices[] = new float[]{
				//Left
        		-1.0f, 1.0f, 1.0f,
	        	-1.0f, 1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, 1.0f,

				//front
  				-1.0f, -1.0f, 1.0f,
       			1.0f, -1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
   				-1.0f, 1.0f, 1.0f,

       			//Right
        		1.0f, 1.0f, 1.0f,
   	   			1.0f, -1.0f, 1.0f,
   				1.0f, -1.0f, -1.0f,
       			1.0f, 1.0f, -1.0f,

				//Back
				1.0f, 1.0f, -1.0f,
       			1.0f, -1.0f, -1.0f,
       			-1.0f, -1.0f, -1.0f,
       			-1.0f, 1.0f, -1.0f,

        		//Top
				1.0f, 1.0f, -1.0f,
   				-1.0f, 1.0f, -1.0f,
   				-1.0f, 1.0f, 1.0f,
       			1.0f, 1.0f, 1.0f,

				//Bottom
				1.0f, -1.0f, -1.0f,
      			-1.0f, -1.0f, -1.0f,
    			-1.0f, -1.0f, 1.0f,
      			1.0f, -1.0f, 1.0f
        	};

		//Color
		final float CubeColor[] = new float[]{

				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,

				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,

				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,

				1.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f,

				1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,

				0.0f, 1.0f, 1.0f,
				0.0f, 1.0f, 1.0f,
				0.0f, 1.0f, 1.0f,
				0.0f, 1.0f, 1.0f
				};

		/************ Pyramid ************************/
		GLES32.glGenVertexArrays(1, Pyramid_vao, 0);
		GLES32.glBindVertexArray(Pyramid_vao[0]);

		// THIS is Vertex VBO
		GLES32.glGenBuffers(1, vbo_Pyramid_Vertex, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Pyramid_Vertex[0]);
		ByteBuffer  byteBuffer = ByteBuffer.allocateDirect(PyramidVertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer vertexBuffer = byteBuffer.asFloatBuffer();
		vertexBuffer.put(PyramidVertices);
		vertexBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
				PyramidVertices.length *4,
				vertexBuffer,
				GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.DVM_ATTRIBUTE_VERTEX,
				3,
				GLES32.GL_FLOAT,
				false,
				0,
				0);
		GLES32.glEnableVertexAttribArray(GLESMacros.DVM_ATTRIBUTE_VERTEX);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		/* This  Color VBO */
		GLES32.glGenBuffers(1, vbo_Pyramid_Color, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Pyramid_Color[0]);
		byteBuffer = ByteBuffer.allocateDirect(PyramidColor.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		vertexBuffer = byteBuffer.asFloatBuffer();
		vertexBuffer.put(PyramidColor);
		vertexBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
				PyramidColor.length *4,
				vertexBuffer,
				GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.DVM_ATTRIBUTE_COLOR,
				3,
				GLES32.GL_FLOAT,
				false,
				0,
				0);
		GLES32.glEnableVertexAttribArray(GLESMacros.DVM_ATTRIBUTE_COLOR);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		GLES32.glBindVertexArray(0);

		/*********************** Cube ********************/ 
		GLES32.glGenVertexArrays(1, Cube_vao, 0);
		GLES32.glBindVertexArray(Cube_vao[0]);

		// THIS is Vertex VBO
		GLES32.glGenBuffers(1, vbo_Cube_Vertex, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Cube_Vertex[0]);
		byteBuffer = ByteBuffer.allocateDirect(CubeVertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		vertexBuffer = byteBuffer.asFloatBuffer();
		vertexBuffer.put(CubeVertices);
		vertexBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
				CubeVertices.length *4,
				vertexBuffer,
				GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.DVM_ATTRIBUTE_VERTEX,
				3,
				GLES32.GL_FLOAT,
				false,
				0,
				0);
		GLES32.glEnableVertexAttribArray(GLESMacros.DVM_ATTRIBUTE_VERTEX);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		/* This  Color VBO */
		GLES32.glGenBuffers(1, vbo_Cube_Color, 0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Cube_Color[0]);
		byteBuffer = ByteBuffer.allocateDirect(CubeColor.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		vertexBuffer = byteBuffer.asFloatBuffer();
		vertexBuffer.put(CubeColor);
		vertexBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
				CubeColor.length *4,
				vertexBuffer,
				GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.DVM_ATTRIBUTE_COLOR,
				3,
				GLES32.GL_FLOAT,
				false,
				0,
				0);
		GLES32.glEnableVertexAttribArray(GLESMacros.DVM_ATTRIBUTE_COLOR);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		GLES32.glBindVertexArray(0);

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);
		GLES32.glClearDepthf(1.0f);
		GLES32.glClearColor(0.0f,0.0f,0.0f, 1.0f);

		Matrix.setIdentityM(perspectivegraphicProjectionMatrix, 0);
	}

	private void resize(int width, int height)
	{
		GLES32.glViewport(0,0, width, height);

		Matrix.perspectiveM( perspectivegraphicProjectionMatrix,
				0,
				44.0f,
				(float)width / (float)height,
				0.1f,
				100.0f );
	}

	private void display()
	{
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		GLES32.glUseProgram(ShaderProgramObject);

		float translateMatrix[] = new float[16];
		float RotationMatrix_X[] = new float[16];
		float RotationMatrix_Y[] = new float[16];
		float RotationMatrix_Z[] = new float[16];
		float modelViewMatrix[] = new float[16];
		float modelViewProjectionMatrix[] = new float[16];

		Matrix.setIdentityM(translateMatrix, 0);
		Matrix.setIdentityM(RotationMatrix_X, 0);
		Matrix.setIdentityM(RotationMatrix_Y, 0);
		Matrix.setIdentityM(RotationMatrix_Z, 0);
		Matrix.setIdentityM(modelViewMatrix, 0);
		Matrix.setIdentityM(modelViewProjectionMatrix, 0);

		Matrix.translateM(translateMatrix, 0, -2.0f, 0.0f, -6.0f);
		Matrix.rotateM(RotationMatrix_Y, 0, angle_p, 0.0f, 1.0f, 0.0f);

		Matrix.multiplyMM(modelViewMatrix,
				0,
				translateMatrix,
				0,
				RotationMatrix_Y,
				0);

		Matrix.multiplyMM(modelViewProjectionMatrix,
				0,
				perspectivegraphicProjectionMatrix,
				0,
				modelViewMatrix,
				0);

		GLES32.glUniformMatrix4fv(mvpUniform, 1,false, modelViewProjectionMatrix,0);

		GLES32.glBindVertexArray(Pyramid_vao[0]);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 12);
		
		GLES32.glBindVertexArray(0);

		Matrix.setIdentityM(translateMatrix, 0);
		Matrix.setIdentityM(RotationMatrix_X, 0);
		Matrix.setIdentityM(RotationMatrix_Y, 0);
		Matrix.setIdentityM(RotationMatrix_Z, 0);
		Matrix.setIdentityM(modelViewMatrix, 0);
		Matrix.setIdentityM(modelViewProjectionMatrix, 0);

		Matrix.translateM(translateMatrix, 0, 2.0f, 0.0f, -6.0f);
		Matrix.rotateM(RotationMatrix_X, 0, angle_p, 1.0f, 0.0f, 0.0f);
		Matrix.rotateM(RotationMatrix_Y, 0, angle_p, 0.0f, 1.0f, 0.0f);
		Matrix.rotateM(RotationMatrix_Z, 0, angle_p, 0.0f, 0.0f, 1.0f);

		Matrix.multiplyMM(RotationMatrix_X,
				0,
				RotationMatrix_Y,
				0,
				RotationMatrix_Z,
				0);

		Matrix.multiplyMM(modelViewMatrix,
				0,
				translateMatrix,
				0,
				RotationMatrix_X,
				0);

		Matrix.multiplyMM(modelViewProjectionMatrix,
				0,
				perspectivegraphicProjectionMatrix,
				0,
				modelViewMatrix,
				0);

		GLES32.glUniformMatrix4fv(mvpUniform, 1,false, modelViewProjectionMatrix,0);

		GLES32.glBindVertexArray(Cube_vao[0]);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);

		GLES32.glBindVertexArray(0);

		GLES32.glUseProgram(0);

		update();

		requestRender();
	}

	private void update()
	{
		angle_p += 1.0f;
		if(angle_p > 360.0f)
		{
			angle_p = 0.0f;
		}
	}
	private void UnInitialized()
	{
		if(Pyramid_vao[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, Pyramid_vao, 0);
			Pyramid_vao[0] = 0;
		}
		if(vbo_Pyramid_Vertex[0] != 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Pyramid_Vertex, 0);
			vbo_Pyramid_Vertex[0] = 0;
		}
		if(vbo_Pyramid_Color[0] != 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Pyramid_Color, 0);
			vbo_Pyramid_Color[0] = 0;
		}

		if(Cube_vao[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, Cube_vao, 0);
			Cube_vao[0] = 0;
		}
		if(vbo_Cube_Vertex[0] != 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Cube_Vertex, 0);
			vbo_Cube_Vertex[0] = 0;
		}
		if(vbo_Cube_Color[0] != 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Cube_Color, 0);
			vbo_Cube_Color[0] = 0;
		}
		if(ShaderProgramObject != 0)
		{
			if(vertexShaderObject != 0)
			{
				GLES32.glDetachShader(ShaderProgramObject, vertexShaderObject);
				GLES32.glDeleteShader(vertexShaderObject);
				vertexShaderObject = 0;
			}
			if(fragmentShaderObject != 0)
			{
				GLES32.glDetachShader(ShaderProgramObject, fragmentShaderObject);
				GLES32.glDeleteShader(fragmentShaderObject);
				fragmentShaderObject = 0;
			}
		}
		if(ShaderProgramObject != 0)
		{
			GLES32.glDeleteProgram(ShaderProgramObject);
			ShaderProgramObject = 0;
		}
	}
}
