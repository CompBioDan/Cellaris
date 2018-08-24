

class CellExample : public CreateScene
{
public:

	CellExample(const char* name) : CreateScene(name) {}

	virtual void Initialize()
	{
		const float mRadius = 0.05f; // Radius of cells forming the fluid
		const float radius = mRadius; 
		const float restDistance = mRadius * 0.65f; // 0.0325 (ratio of radius: rest distance ~ 2:1)
		const float mDomainSize = 2.0f;

		int dx = int(ceilf(mDomainSize / restDistance)); // number of cells along x direction (31)
		int dy = int(ceilf(mDomainSize / restDistance)); // number of cells along y direction (62)
		int dz = int(ceilf(mDomainSize / restDistance)); // number of cells along z direction (31)

		CreateParticleGrid(Vec3(0.0f, restDistance, 0.0f), dx, dy, dz, restDistance, Vec3(0.0f), 1.0f, false, 0.0f, NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid), restDistance*0.01f);

		g_sceneLower = Vec3(0.0f, 0.0f, 0.0f);
		g_sceneUpper = Vec3(mDomainSize, mDomainSize, mDomainSize);

		g_numSubsteps = 2;

		g_params.fluid = true;
		g_params.radius = radius;
		g_params.fluidRestDistance = restDistance;
		g_params.dynamicFriction = 0.f;
		g_params.restitution = 0.001f;

		g_params.numIterations = 3;
		g_params.relaxationFactor = 1.0f;

		g_params.smoothing = 0.4f;
		g_params.anisotropyScale = 3.0f / radius;

		g_params.viscosity = 0.001f;
		g_params.cohesion = 0.1f;
		g_params.vorticityConfinement = 80.0f;
		g_params.surfaceTension = 0.0f;
		g_params.gravity[1] = 0.0f;
		g_params.numPlanes =6;

		// limit velocity to CFL condition
		g_params.maxSpeed = 0.5f*radius*g_numSubsteps / g_dt;

		g_maxDiffuseParticles = 0;

		g_fluidColor = Vec4(0.113f, 0.425f, 0.55f, 1.0f);

		g_waveFrequency = 1.0f;
		g_waveAmplitude = 2.0f;
		g_waveFloorTilt = 0.0f;

		// draw options		
		g_drawPoints = true;
		g_drawEllipsoids = false;
		g_drawDiffuse = true;
	}

	float mRadius;

	void CreateParticleGrid(Vec3 lower, int dimx, int dimy, int dimz, float radius, Vec3 velocity, float invMass, bool rigid, float rigidStiffness, int phase, float jitter = 0.005f)
	{
		if (rigid && g_buffers->rigidIndices.empty())
			g_buffers->rigidOffsets.push_back(0);

		for (int x = 0; x < dimx; ++x)
		{
			for (int y = 0; y < dimy; ++y)
			{
				for (int z = 0; z < dimz; ++z)
				{
					if (rigid)
						g_buffers->rigidIndices.push_back(int(g_buffers->positions.size()));

					Vec3 position = lower + Vec3(float(x), float(y), float(z))*radius + RandomUnitVector()*jitter;

					g_buffers->positions.push_back(Vec4(position.x, position.y, position.z, invMass));
					g_buffers->velocities.push_back(velocity);
					g_buffers->phases.push_back(phase);
				}
			}
		}

		if (rigid)
		{
			g_buffers->rigidCoefficients.push_back(rigidStiffness);
			g_buffers->rigidOffsets.push_back(int(g_buffers->rigidIndices.size()));
		}
	}
};
