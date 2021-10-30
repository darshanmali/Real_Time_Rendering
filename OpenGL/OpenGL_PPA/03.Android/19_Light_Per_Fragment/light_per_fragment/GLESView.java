
package com.darshan_vilas_mali.light_per_fragment;

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
import java.nio.ShortBuffer;

import android.opengl.Matrix;


public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,OnGestureListener, OnDoubleTapListener{

	private final Context context;

	private class Light{

		float[] Light_A = new float[3];
		float[] Light_D = new float[3];
		float[] Light_S = new float[3];
		float[] Light_P = new float[4];
	};

	private class Material{
		float[] Material_A = new float[3];
		float[] Material_D = new float[3];
		float[] Material_S = new float[3];
		float[] Material_Shininess = new float[1];
	};

	private Light light[] = new Light[1];
	private Material materials[] = new Material[1];

	private int[] lSingle_Tap_DM = new int[1];

	private int[] LA_Uniform_DM = new int[1];
	private int[] LD_Uniform_DM = new int[1];
	private int[] LS_Uniform_DM = new int[1];
	private int[] LPosition_Uniform_DM = new int[1];

	private int[] KA_Uniform_DM = new int[1];
	private int[] KD_Uniform_DM = new int[1];
	private int[] KS_Uniform_DM = new int[1];
	private int[] Shininess_DM = new int[1];

	private Boolean bAnimation = false;
	private Boolean bLight_sWitch = false;
	private GestureDetector gestureDetector;

	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int ShaderProgramObject;

	private int mUniform_Matrix;
	private int vUniform_Matrix;
	private int pUniform_Matrix;

	private float angle_p =0.0f;

	private float perspectivegraphicProjectionMatrix[] = new float[16];

	/* Sphere Variables */
	private int[] vao_sphere = new int[1];
	private int[] vbo_sphere_position = new int[1];
	private int[] vbo_sphere_normal = new int[1];
	private int[] vbo_sphere_element = new int[1];

	private int numVertices ;
	private int numElements ;

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
		if(bAnimation){
			bAnimation = false;
		}
		else{
			bAnimation = true;
		}
		return true;
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e) {
		return true;
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e) {
		System.out.println("DVM: Single Tap");
		if(bLight_sWitch){
			bLight_sWitch = false;
		}
		else{
			bLight_sWitch = true;
		}
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
		light[0] = new Light();

		light[0].Light_A[0] = 0.0f;
		light[0].Light_A[1] = 0.0f;
		light[0].Light_A[2] = 0.0f;

		light[0].Light_D[0] = 1.0f;
		light[0].Light_D[1] = 1.0f;
		light[0].Light_D[2] = 1.0f;

		light[0].Light_S[0] = 1.0f;
		light[0].Light_S[1] = 1.0f;
		light[0].Light_S[2] = 1.0f;

		light[0].Light_P[0] = 100.0f;
		light[0].Light_P[1] = 100.0f;
		light[0].Light_P[2] = 100.0f;
		light[0].Light_P[3] = 1.0f;

		materials[0] = new Material();
		materials[0].Material_A[0] = 0.0f;
		materials[0].Material_A[1] = 0.0f;
		materials[0].Material_A[2] = 0.0f;

		materials[0].Material_D[0] = 1.0f;
		materials[0].Material_D[1] = 1.0f;
		materials[0].Material_D[2] = 1.0f;

		materials[0].Material_S[0] = 1.0f;
		materials[0].Material_S[1] = 1.0f;
		materials[0].Material_S[2] = 1.0f;

		materials[0].Material_Shininess[0] = 50.0f;


		Sphere sphere=new Sphere();
		float sphere_vertices[]=new float[1146];
		float sphere_normals[]=new float[1146];
		float sphere_textures[]=new float[764];
		short sphere_elements[]=new short[2280];

		/* Vertex Shader */

		vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		final String vertexShaderSourceCode = String.format(
				"#version 320 es\n" +
				"precision highp float;\n" +
				"precision highp int;\n" +
				"in vec4 vPosition;" +
				"in vec3 vNormal;" +
				"uniform mat4 u_model_matrix;" +
				"uniform mat4 u_view_matrix;" +
				"uniform mat4 u_p_matrix;" +
				"uniform int u_lKey;" +
				"uniform vec4 u_light_position;" +
				"out vec3 transformed_normal;"  +
				"out vec3 light_direction;\n" +
				"out vec3 view_vector;\n" +
				"void main(void){\n" +
				"if(u_lKey == 1){\n" +
				"vec4 eye_cordinates = normalize(vec4(u_view_matrix * u_model_matrix * vPosition));\n" +
				"transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" +
				"light_direction = normalize(vec3(u_light_position - eye_cordinates));\n" +
				"view_vector = normalize(-eye_cordinates.xyz );\n" +
				"}\n" +
				"gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" +
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
				"precision highp float;\n" +
				"precision highp int;\n" +
				"uniform int u_lKey;" +
				"uniform vec3 u_la;" +
				"uniform vec3 u_ld;" +
				"uniform vec3 u_ls;" +
				"uniform vec3 u_ka;" +
				"uniform vec3 u_kd;" +
				"uniform vec3 u_ks;" +
				"uniform float u_shininess;\n" +
				"in vec3 transformed_normal;\n"  +
				"in vec3 light_direction;\n" +
				"in vec3 view_vector;\n" +
				"out vec4 FragColor;\n" +
				"void main(void){\n" +
				"if(u_lKey == 1){\n" +
				"vec3 normalized_transformed_normal = transformed_normal;\n" +
				"vec3 normalized_light_direction = light_direction;\n" +
				"vec3 normalized_view_vector = view_vector;\n" +
				"vec3 reflection_vector = reflect(-normalized_light_direction , normalized_transformed_normal);\n" +
				"vec3 ambient = u_la * u_ka;\n" +
				"vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normal), 0.0f);\n" +
				"vec3 specular = u_ls * u_ks * pow( max(dot(reflection_vector, normalized_view_vector), 0.0f), u_shininess);\n" +
				"vec3 fong_ads_light = ambient + diffuse + specular;\n" +
				"FragColor = vec4(fong_ads_light, 1.0f);\n"+
				"}\n" +
				"else{\n" +
				"FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"+
				"}\n" +
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
		GLES32.glBindAttribLocation(ShaderProgramObject, GLESMacros.DVM_ATTRIBUTE_NORMAL,"vNormal");

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

		mUniform_Matrix = GLES32.glGetUniformLocation( ShaderProgramObject, "u_model_matrix");
		vUniform_Matrix = GLES32.glGetUniformLocation( ShaderProgramObject, "u_view_matrix");
		pUniform_Matrix = GLES32.glGetUniformLocation( ShaderProgramObject, "u_p_matrix");

		lSingle_Tap_DM[0] = GLES32.glGetUniformLocation( ShaderProgramObject, "u_lKey");

		LA_Uniform_DM[0] = GLES32.glGetUniformLocation( ShaderProgramObject, "u_la");
		LD_Uniform_DM[0] = GLES32.glGetUniformLocation( ShaderProgramObject, "u_ld");
		LS_Uniform_DM[0] = GLES32.glGetUniformLocation( ShaderProgramObject, "u_ls");
		LPosition_Uniform_DM[0] = GLES32.glGetUniformLocation( ShaderProgramObject, "u_light_position");

		KA_Uniform_DM[0] = GLES32.glGetUniformLocation( ShaderProgramObject, "u_ka");
		KD_Uniform_DM[0] = GLES32.glGetUniformLocation( ShaderProgramObject, "u_kd");
		KS_Uniform_DM[0] = GLES32.glGetUniformLocation( ShaderProgramObject, "u_ks");
		Shininess_DM[0] = GLES32.glGetUniformLocation( ShaderProgramObject, "u_shininess");

		sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
		numVertices = sphere.getNumberOfSphereVertices();
		numElements = sphere.getNumberOfSphereElements();

		// vao
		GLES32.glGenVertexArrays(1,vao_sphere,0);
		GLES32.glBindVertexArray(vao_sphere[0]);

		// position vbo
		GLES32.glGenBuffers(1,vbo_sphere_position,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_sphere_position[0]);

		ByteBuffer byteBuffer=ByteBuffer.allocateDirect(sphere_vertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer=byteBuffer.asFloatBuffer();
		verticesBuffer.put(sphere_vertices);
		verticesBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
				sphere_vertices.length * 4,
				verticesBuffer,
				GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.DVM_ATTRIBUTE_VERTEX,
				3,
				GLES32.GL_FLOAT,
				false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.DVM_ATTRIBUTE_VERTEX);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

		// normal vbo
		GLES32.glGenBuffers(1,vbo_sphere_normal,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_sphere_normal[0]);

		byteBuffer=ByteBuffer.allocateDirect(sphere_normals.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		verticesBuffer=byteBuffer.asFloatBuffer();
		verticesBuffer.put(sphere_normals);
		verticesBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
				sphere_normals.length * 4,
				verticesBuffer,
				GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.DVM_ATTRIBUTE_NORMAL,
				3,
				GLES32.GL_FLOAT,
				false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.DVM_ATTRIBUTE_NORMAL);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

		// element vbo
		GLES32.glGenBuffers(1,vbo_sphere_element,0);
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER,vbo_sphere_element[0]);

		byteBuffer=ByteBuffer.allocateDirect(sphere_elements.length * 2);
		byteBuffer.order(ByteOrder.nativeOrder());
		ShortBuffer elementsBuffer=byteBuffer.asShortBuffer();
		elementsBuffer.put(sphere_elements);
		elementsBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER,
				sphere_elements.length * 2,
				elementsBuffer,
				GLES32.GL_STATIC_DRAW);

		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER,0);

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

		if(bLight_sWitch)
		{
			GLES32.glUniform1i(lSingle_Tap_DM[0], 1);
			GLES32.glUniform3fv(LA_Uniform_DM[0],1,light[0].Light_A, 0);
			GLES32.glUniform3fv(LD_Uniform_DM[0],1,light[0].Light_D, 0);
			GLES32.glUniform3fv(LS_Uniform_DM[0],1,light[0].Light_S, 0);
			GLES32.glUniform4fv(LPosition_Uniform_DM[0],1,light[0].Light_P, 0);

			GLES32.glUniform3fv(KA_Uniform_DM[0],1,materials[0].Material_A, 0);
			GLES32.glUniform3fv(KD_Uniform_DM[0],1,materials[0].Material_D, 0);
			GLES32.glUniform3fv(KS_Uniform_DM[0],1,materials[0].Material_S, 0);
			GLES32.glUniform1f(Shininess_DM[0],materials[0].Material_Shininess[0]);
		}
		else
		{
			GLES32.glUniform1i(lSingle_Tap_DM[0], 0);
		}

		float translateMatrix[] = new float[16];
		float RotationMatrix[] = new float[16];
		float modelMatrix[] = new float[16];
		float ViewMatrix[] = new float[16];
		float ProjectionMatrix[] = new float[16];

		Matrix.setIdentityM(translateMatrix, 0);
		Matrix.setIdentityM(RotationMatrix, 0);
		Matrix.setIdentityM(ViewMatrix, 0);
		Matrix.setIdentityM(modelMatrix, 0);
		Matrix.setIdentityM(ProjectionMatrix, 0);

		Matrix.translateM(translateMatrix, 0, 0.0f, 0.0f,-2.0f);
		Matrix.rotateM(RotationMatrix, 0, angle_p, 0.0f, 1.0f, 0.0f);

		Matrix.multiplyMM(modelMatrix,
				0,
				translateMatrix,
				0,
				RotationMatrix,
				0);

		GLES32.glUniformMatrix4fv(vUniform_Matrix, 1,false, ViewMatrix,0);
		GLES32.glUniformMatrix4fv(mUniform_Matrix, 1,false, modelMatrix,0);
		GLES32.glUniformMatrix4fv(pUniform_Matrix, 1,false, perspectivegraphicProjectionMatrix,0);

		GLES32.glBindVertexArray(vao_sphere[0]);

		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		// unbind vao
		GLES32.glBindVertexArray(0);

		GLES32.glUseProgram(0);

		if(bAnimation)
		{
			update();
		}
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
		// destroy vao
		if(vao_sphere[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
			vao_sphere[0]=0;
		}

		// destroy position vbo
		if(vbo_sphere_position[0] != 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_position, 0);
			vbo_sphere_position[0]=0;
		}

		// destroy normal vbo
		if(vbo_sphere_normal[0] != 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_normal, 0);
			vbo_sphere_normal[0]=0;
		}

		// destroy element vbo
		if(vbo_sphere_element[0] != 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
			vbo_sphere_element[0]=0;
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
