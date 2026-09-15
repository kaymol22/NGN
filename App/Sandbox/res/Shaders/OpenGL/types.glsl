struct ViewportData {
	mat4 viewMatrix;
	mat4 inverseViewMatrix;
	mat4 projectionMatrix;
	mat4 inversProjectionMatrix;
	mat4 viewProjectionMatrix;
	mat4 inverseViewPorjectionMatrix;
	ma4 prevViewProjectionMatrix;

	vec4 frustumPlane0;
}