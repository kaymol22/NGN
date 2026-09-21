struct ViewportData {
	mat4 viewMatrix;
	mat4 inverseViewMatrix;
	mat4 projectionMatrix;
	mat4 inverseProjectionMatrix;
	mat4 viewProjectionMatrix;
	mat4 inverseViewProjectionMatrix;

	vec4 viewPosition;
	vec4 camForward;
	vec4 camUp;
	vec4 camRight;

	float fov;
	float nearPlane;
	float farPlane;
	float orthographicSize;
};

struct RenderItem {
	mat4 transform;
	vec4 color;
};