

#include "flexHelper.h"
#include "flexSetup.h"

void flexHelper::Init(int scene, bool centerCamera = true)
{
	RandInit(); // Seed random number?

	if (g_flex) // If instance of solver already exists remove it
	{
		if (g_buffers)
			DestroyBuffers(g_buffers);

		DestroyFluidRenderBuffers(g_fluidRenderBuffers);
		DestroyDiffuseRenderBuffers(g_diffuseRenderBuffers);

		for (auto& iter : g_meshes)
		{
			NvFlexDestroyTriangleMesh(g_flexLib, iter.first);
			DestroyGpuMesh(iter.second);
		}

		for (auto& iter : g_fields)
		{
			NvFlexDestroyDistanceField(g_flexLib, iter.first);
			DestroyGpuMesh(iter.second);
		}

		for (auto& iter : g_convexes)
		{
			NvFlexDestroyConvexMesh(g_flexLib, iter.first);
			DestroyGpuMesh(iter.second);
		}


		g_fields.clear();
		g_meshes.clear();
		g_convexes.clear();

		NvFlexDestroySolver(g_flex);
		g_flex = NULL;
	}

	// alloc buffers
	g_buffers = AllocBuffers(g_flexLib);

	// map during initialization
	MapBuffers(g_buffers);

	g_buffers->positions.resize(0);
	g_buffers->velocities.resize(0);
	g_buffers->phases.resize(0);

	g_buffers->rigidOffsets.resize(0);
	g_buffers->rigidIndices.resize(0);
	g_buffers->rigidMeshSize.resize(0);
	g_buffers->rigidRotations.resize(0);
	g_buffers->rigidTranslations.resize(0);
	g_buffers->rigidCoefficients.resize(0);
	g_buffers->rigidLocalPositions.resize(0);
	g_buffers->rigidLocalNormals.resize(0);

	g_buffers->springIndices.resize(0);
	g_buffers->springLengths.resize(0);
	g_buffers->springStiffness.resize(0);
	g_buffers->triangles.resize(0);
	g_buffers->triangleNormals.resize(0);
	g_buffers->uvs.resize(0);

	g_meshSkinIndices.resize(0);
	g_meshSkinWeights.resize(0);

	g_emitters.resize(1);
	g_emitters[0].mEnabled = false;
	g_emitters[0].mSpeed = 1.0f;

	g_buffers->shapeGeometry.resize(0);
	g_buffers->shapePositions.resize(0);
	g_buffers->shapeRotations.resize(0);
	g_buffers->shapePrevPositions.resize(0);
	g_buffers->shapePrevRotations.resize(0);
	g_buffers->shapeFlags.resize(0);

	g_ropes.resize(0);

	// remove collision shapes
	delete g_mesh; g_mesh = NULL;

	g_frame = 0;
	g_pause = false;

	g_dt = 1.0f / 60.0f;
	g_waveTime = 0.0f;
	g_windTime = 0.0f;
	g_windStrength = 1.0f;

	g_blur = 1.0f;
	g_fluidColor = Vec4(0.1f, 0.4f, 0.8f, 1.0f);
	g_meshColor = Vec3(0.9f, 0.9f, 0.9f);
	g_drawEllipsoids = false;
	g_drawPoints = true;
	g_drawCloth = true;
	g_expandCloth = 0.0f;

	g_drawOpaque = false;
	g_drawSprings = false;
	g_drawDiffuse = false;
	g_drawMesh = true;
	g_drawRopes = true;
	g_drawDensity = false;
	g_ior = 1.0f;
	g_lightDistance = 2.0f;
	g_fogDistance = 0.005f;

	g_camSpeed = 0.075f;
	g_camNear = 0.01f;
	g_camFar = 1000.0f;

	g_pointScale = 1.0f;
	g_ropeScale = 1.0f;
	g_drawPlaneBias = 0.0f;

	// sim params
	g_params.gravity[0] = 0.0f;
	g_params.gravity[1] = -9.8f;
	g_params.gravity[2] = 0.0f;

	g_params.wind[0] = 0.0f;
	g_params.wind[1] = 0.0f;
	g_params.wind[2] = 0.0f;

	g_params.radius = 0.15f;
	g_params.viscosity = 0.0f;
	g_params.dynamicFriction = 0.0f;
	g_params.staticFriction = 0.0f;
	g_params.particleFriction = 0.0f; // scale friction between particles by default
	g_params.freeSurfaceDrag = 0.0f;
	g_params.drag = 0.0f;
	g_params.lift = 0.0f;
	g_params.numIterations = 3;
	g_params.fluidRestDistance = 0.0f;
	g_params.solidRestDistance = 0.0f;

	g_params.anisotropyScale = 1.0f;
	g_params.anisotropyMin = 0.1f;
	g_params.anisotropyMax = 2.0f;
	g_params.smoothing = 1.0f;

	g_params.dissipation = 0.0f;
	g_params.damping = 0.0f;
	g_params.particleCollisionMargin = 0.0f;
	g_params.shapeCollisionMargin = 0.0f;
	g_params.collisionDistance = 0.0f;
	g_params.plasticThreshold = 0.0f;
	g_params.plasticCreep = 0.0f;
	g_params.fluid = false;
	g_params.sleepThreshold = 0.0f;
	g_params.shockPropagation = 0.0f;
	g_params.restitution = 0.0f;

	g_params.maxSpeed = FLT_MAX;
	g_params.maxAcceleration = 100.0f;	// approximately 10x gravity

	g_params.relaxationMode = eNvFlexRelaxationLocal;
	g_params.relaxationFactor = 1.0f;
	g_params.solidPressure = 1.0f;
	g_params.adhesion = 0.0f;
	g_params.cohesion = 0.025f;
	g_params.surfaceTension = 0.0f;
	g_params.vorticityConfinement = 0.0f;
	g_params.buoyancy = 1.0f;
	g_params.diffuseThreshold = 100.0f;
	g_params.diffuseBuoyancy = 1.0f;
	g_params.diffuseDrag = 0.8f;
	g_params.diffuseBallistic = 16;
	g_params.diffuseSortAxis[0] = 0.0f;
	g_params.diffuseSortAxis[1] = 0.0f;
	g_params.diffuseSortAxis[2] = 0.0f;
	g_params.diffuseLifetime = 2.0f;

	g_numSubsteps = 2;

	// planes created after particles
	g_params.numPlanes = 1;

	g_diffuseScale = 0.5f;
	g_diffuseColor = 1.0f;
	g_diffuseMotionScale = 1.0f;
	g_diffuseShadow = false;
	g_diffuseInscatter = 0.8f;
	g_diffuseOutscatter = 0.53f;

	// reset phase 0 particle color to blue
	extern Colour gColors[];
	gColors[0] = Colour(0.0f, 0.5f, 1.0f);

	g_numSolidParticles = 0;

	g_waveFrequency = 1.5f;
	g_waveAmplitude = 1.5f;
	g_waveFloorTilt = 0.0f;
	g_emit = false;
	g_warmup = false;

	g_mouseParticle = -1;

	g_maxDiffuseParticles = 0;	// number of diffuse particles
	g_maxNeighborsPerParticle = 96;
	g_numExtraParticles = 0;	// number of particles allocated but not made active	

	g_sceneLower = FLT_MAX;
	g_sceneUpper = -FLT_MAX;

	// create scene
	g_scenes[g_scene]->Initialize();

	uint32_t numParticles = g_buffers->positions.size();
	uint32_t maxParticles = numParticles + g_numExtraParticles * g_numExtraMultiplier;

	// by default solid particles use the maximum radius
	if (g_params.fluid && g_params.solidRestDistance == 0.0f)
		g_params.solidRestDistance = g_params.fluidRestDistance;
	else
		g_params.solidRestDistance = g_params.radius;

	// collision distance with shapes half the radius
	if (g_params.collisionDistance == 0.0f)
	{
		g_params.collisionDistance = g_params.radius*0.5f;

		if (g_params.fluid)
			g_params.collisionDistance = g_params.fluidRestDistance*0.5f;
	}

	// default particle friction to 10% of shape friction
	if (g_params.particleFriction == 0.0f)
		g_params.particleFriction = g_params.dynamicFriction*0.1f;

	// add a margin for detecting contacts between particles and shapes
	if (g_params.shapeCollisionMargin == 0.0f)
		g_params.shapeCollisionMargin = g_params.collisionDistance*0.5f;

	// calculate particle bounds
	Vec3 particleLower, particleUpper;
	GetParticleBounds(particleLower, particleUpper);

	// accommodate shapes
	Vec3 shapeLower, shapeUpper;
	GetShapeBounds(shapeLower, shapeUpper);

	// update bounds
	g_sceneLower = Min(Min(g_sceneLower, particleLower), shapeLower);
	g_sceneUpper = Max(Max(g_sceneUpper, particleUpper), shapeUpper);

	g_sceneLower -= g_params.collisionDistance;
	g_sceneUpper += g_params.collisionDistance;

	// update collision planes to match flexs
	Vec3 up = Normalize(Vec3(-g_waveFloorTilt, 1.0f, 0.0f));

	(Vec4&)g_params.planes[0] = Vec4(up.x, up.y, up.z, 0.0f);
	(Vec4&)g_params.planes[1] = Vec4(0.0f, 0.0f, 1.0f, -g_sceneLower.z);
	(Vec4&)g_params.planes[2] = Vec4(1.0f, 0.0f, 0.0f, -g_sceneLower.x);
	(Vec4&)g_params.planes[3] = Vec4(-1.0f, 0.0f, 0.0f, g_sceneUpper.x);
	(Vec4&)g_params.planes[4] = Vec4(0.0f, 0.0f, -1.0f, g_sceneUpper.z);
	(Vec4&)g_params.planes[5] = Vec4(0.0f, -1.0f, 0.0f, g_sceneUpper.y);

	g_wavePlane = g_params.planes[2][3];

	g_buffers->diffusePositions.resize(g_maxDiffuseParticles);
	g_buffers->diffuseVelocities.resize(g_maxDiffuseParticles);
	g_buffers->diffuseIndices.resize(g_maxDiffuseParticles);

	// for fluid rendering these are the Laplacian smoothed positions
	g_buffers->smoothPositions.resize(maxParticles);

	g_buffers->normals.resize(0);
	g_buffers->normals.resize(maxParticles);

	// initialize normals (just for rendering before simulation starts)
	int numTris = g_buffers->triangles.size() / 3;
	for (int i = 0; i < numTris; ++i)
	{
		Vec3 v0 = Vec3(g_buffers->positions[g_buffers->triangles[i * 3 + 0]]);
		Vec3 v1 = Vec3(g_buffers->positions[g_buffers->triangles[i * 3 + 1]]);
		Vec3 v2 = Vec3(g_buffers->positions[g_buffers->triangles[i * 3 + 2]]);

		Vec3 n = Cross(v1 - v0, v2 - v0);

		g_buffers->normals[g_buffers->triangles[i * 3 + 0]] += Vec4(n, 0.0f);
		g_buffers->normals[g_buffers->triangles[i * 3 + 1]] += Vec4(n, 0.0f);
		g_buffers->normals[g_buffers->triangles[i * 3 + 2]] += Vec4(n, 0.0f);
	}

	for (int i = 0; i < int(maxParticles); ++i)
		g_buffers->normals[i] = Vec4(SafeNormalize(Vec3(g_buffers->normals[i]), Vec3(0.0f, 1.0f, 0.0f)), 0.0f);


	// save mesh positions for skinning
	if (g_mesh)
	{
		g_meshRestPositions = g_mesh->m_positions;
	}
	else
	{
		g_meshRestPositions.resize(0);
	}

	// main create method for the Flex solver
	g_flex = NvFlexCreateSolver(g_flexLib, maxParticles, g_maxDiffuseParticles, g_maxNeighborsPerParticle);

	// give scene a chance to do some post solver initialization
	g_scenes[g_scene]->PostInitialize();

	// center camera on particles
	if (centerCamera)
	{
		g_camPos = Vec3((g_sceneLower.x + g_sceneUpper.x)*0.5f, min(g_sceneUpper.y*1.25f, 6.0f), g_sceneUpper.z + min(g_sceneUpper.y, 6.0f)*2.0f);
		g_camAngle = Vec3(0.0f, -DegToRad(15.0f), 0.0f);

		// give scene a chance to modify camera position
		g_scenes[g_scene]->CenterCamera();
	}

	// create active indices (just a contiguous block for the demo)
	g_buffers->activeIndices.resize(g_buffers->positions.size());
	for (int i = 0; i < g_buffers->activeIndices.size(); ++i)
		g_buffers->activeIndices[i] = i;

	// resize particle buffers to fit
	g_buffers->positions.resize(maxParticles);
	g_buffers->velocities.resize(maxParticles);
	g_buffers->phases.resize(maxParticles);

	g_buffers->densities.resize(maxParticles);
	g_buffers->anisotropy1.resize(maxParticles);
	g_buffers->anisotropy2.resize(maxParticles);
	g_buffers->anisotropy3.resize(maxParticles);

	// save rest positions
	g_buffers->restPositions.resize(g_buffers->positions.size());
	for (int i = 0; i < g_buffers->positions.size(); ++i)
		g_buffers->restPositions[i] = g_buffers->positions[i];

	// builds rigids constraints
	if (g_buffers->rigidOffsets.size())
	{
		assert(g_buffers->rigidOffsets.size() > 1);

		const int numRigids = g_buffers->rigidOffsets.size() - 1;

		// calculate local rest space positions
		g_buffers->rigidLocalPositions.resize(g_buffers->rigidOffsets.back());
		CalculateRigidLocalPositions(&g_buffers->positions[0], g_buffers->positions.size(), &g_buffers->rigidOffsets[0], &g_buffers->rigidIndices[0], numRigids, &g_buffers->rigidLocalPositions[0]);

		g_buffers->rigidRotations.resize(g_buffers->rigidOffsets.size() - 1, Quat());
		g_buffers->rigidTranslations.resize(g_buffers->rigidOffsets.size() - 1, Vec3());

	}

	// unmap so we can start transferring data to GPU
	UnmapBuffers(g_buffers);

	//-----------------------------
	// Send data to Flex

	NvFlexSetParams(g_flex, &g_params);
	NvFlexSetParticles(g_flex, g_buffers->positions.buffer, numParticles);
	NvFlexSetVelocities(g_flex, g_buffers->velocities.buffer, numParticles);
	NvFlexSetNormals(g_flex, g_buffers->normals.buffer, numParticles);
	NvFlexSetPhases(g_flex, g_buffers->phases.buffer, g_buffers->phases.size());
	NvFlexSetRestParticles(g_flex, g_buffers->restPositions.buffer, g_buffers->restPositions.size());

	NvFlexSetActive(g_flex, g_buffers->activeIndices.buffer, numParticles);

	// springs
	if (g_buffers->springIndices.size())
	{
		assert((g_buffers->springIndices.size() & 1) == 0);
		assert((g_buffers->springIndices.size() / 2) == g_buffers->springLengths.size());

		NvFlexSetSprings(g_flex, g_buffers->springIndices.buffer, g_buffers->springLengths.buffer, g_buffers->springStiffness.buffer, g_buffers->springLengths.size());
	}

	// rigids
	if (g_buffers->rigidOffsets.size())
	{
		NvFlexSetRigids(g_flex, g_buffers->rigidOffsets.buffer, g_buffers->rigidIndices.buffer, g_buffers->rigidLocalPositions.buffer, g_buffers->rigidLocalNormals.buffer, g_buffers->rigidCoefficients.buffer, g_buffers->rigidRotations.buffer, g_buffers->rigidTranslations.buffer, g_buffers->rigidOffsets.size() - 1, g_buffers->rigidIndices.size());
	}

	// inflatables
	if (g_buffers->inflatableTriOffsets.size())
	{
		NvFlexSetInflatables(g_flex, g_buffers->inflatableTriOffsets.buffer, g_buffers->inflatableTriCounts.buffer, g_buffers->inflatableVolumes.buffer, g_buffers->inflatablePressures.buffer, g_buffers->inflatableCoefficients.buffer, g_buffers->inflatableTriOffsets.size());
	}

	// dynamic triangles
	if (g_buffers->triangles.size())
	{
		NvFlexSetDynamicTriangles(g_flex, g_buffers->triangles.buffer, g_buffers->triangleNormals.buffer, g_buffers->triangles.size() / 3);
	}

	// collision shapes
	if (g_buffers->shapeFlags.size())
	{
		NvFlexSetShapes(
			g_flex,
			g_buffers->shapeGeometry.buffer,
			g_buffers->shapePositions.buffer,
			g_buffers->shapeRotations.buffer,
			g_buffers->shapePrevPositions.buffer,
			g_buffers->shapePrevRotations.buffer,
			g_buffers->shapeFlags.buffer,
			int(g_buffers->shapeFlags.size()));
	}

	// create render buffers
	g_fluidRenderBuffers = CreateFluidRenderBuffers(maxParticles, g_interop);
	g_diffuseRenderBuffers = CreateDiffuseRenderBuffers(g_maxDiffuseParticles, g_interop);

	// perform initial sim warm up
	if (g_warmup)
	{
		printf("Warming up sim..\n");

		// warm it up (relax positions to reach rest density without affecting velocity)
		NvFlexParams copy = g_params;
		copy.numIterations = 4;

		NvFlexSetParams(g_flex, &copy);

		const int kWarmupIterations = 100;

		for (int i = 0; i < kWarmupIterations; ++i)
		{
			NvFlexUpdateSolver(g_flex, 0.0001f, 1, false);
			NvFlexSetVelocities(g_flex, g_buffers->velocities.buffer, maxParticles);
		}

		// udpate host copy
		NvFlexGetParticles(g_flex, g_buffers->positions.buffer, g_buffers->positions.size());
		NvFlexGetSmoothParticles(g_flex, g_buffers->smoothPositions.buffer, g_buffers->smoothPositions.size());
		NvFlexGetAnisotropy(g_flex, g_buffers->anisotropy1.buffer, g_buffers->anisotropy2.buffer, g_buffers->anisotropy3.buffer);

		printf("Finished warm up.\n");
	}
}

void flexHelper::Shutdown()
{
	// free buffers
	DestroyBuffers(g_buffers);

	for (auto& iter : g_meshes)
	{
		NvFlexDestroyTriangleMesh(g_flexLib, iter.first);
		DestroyGpuMesh(iter.second);
	}

	for (auto& iter : g_fields)
	{
		NvFlexDestroyDistanceField(g_flexLib, iter.first);
		DestroyGpuMesh(iter.second);
	}

	for (auto& iter : g_convexes)
	{
		NvFlexDestroyConvexMesh(g_flexLib, iter.first);
		DestroyGpuMesh(iter.second);
	}

	g_fields.clear();
	g_meshes.clear();

	NvFlexDestroySolver(g_flex);
	NvFlexShutdown(g_flexLib);

#if _WIN32
	if (g_ffmpeg)
		_pclose(g_ffmpeg);
#endif
}

void flexHelper::Reset()
{
	Init(g_scene, false);
}

void flexHelper::UpdateEmitters()
{
	float spin = DegToRad(15.0f);

	const Vec3 forward(-sinf(g_camAngle.x + spin)*cosf(g_camAngle.y), sinf(g_camAngle.y), -cosf(g_camAngle.x + spin)*cosf(g_camAngle.y));
	const Vec3 right(Normalize(Cross(forward, Vec3(0.0f, 1.0f, 0.0f))));

	g_emitters[0].mDir = Normalize(forward + Vec3(0.0, 0.4f, 0.0f));
	g_emitters[0].mRight = right;
	g_emitters[0].mPos = g_camPos + forward * 1.f + Vec3(0.0f, 0.2f, 0.0f) + right * 0.65f;

	// process emitters
	if (g_emit)
	{
		int activeCount = NvFlexGetActiveCount(g_flex);

		size_t e = 0;

		// skip camera emitter when moving forward or things get messy
		if (g_camSmoothVel.z >= 0.025f)
			e = 1;

		for (; e < g_emitters.size(); ++e)
		{
			if (!g_emitters[e].mEnabled)
				continue;

			Vec3 emitterDir = g_emitters[e].mDir;
			Vec3 emitterRight = g_emitters[e].mRight;
			Vec3 emitterPos = g_emitters[e].mPos;

			float r;
			int phase;

			if (g_params.fluid)
			{
				r = g_params.fluidRestDistance;
				phase = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid);
			}
			else
			{
				r = g_params.solidRestDistance;
				phase = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide);
			}

			float numParticles = (g_emitters[e].mSpeed / r)*g_dt;

			// whole number to emit
			int n = int(numParticles + g_emitters[e].mLeftOver);

			if (n)
				g_emitters[e].mLeftOver = (numParticles + g_emitters[e].mLeftOver) - n;
			else
				g_emitters[e].mLeftOver += numParticles;

			// create a grid of particles (n particles thick)
			for (int k = 0; k < n; ++k)
			{
				int emitterWidth = g_emitters[e].mWidth;
				int numParticles = emitterWidth * emitterWidth;
				for (int i = 0; i < numParticles; ++i)
				{
					float x = float(i%emitterWidth) - float(emitterWidth / 2);
					float y = float((i / emitterWidth) % emitterWidth) - float(emitterWidth / 2);

					if ((sqr(x) + sqr(y)) <= (emitterWidth / 2)*(emitterWidth / 2))
					{
						Vec3 up = Normalize(Cross(emitterDir, emitterRight));
						Vec3 offset = r * (emitterRight*x + up * y) + float(k)*emitterDir*r;

						if (activeCount < g_buffers->positions.size())
						{
							g_buffers->positions[activeCount] = Vec4(emitterPos + offset, 1.0f);
							g_buffers->velocities[activeCount] = emitterDir * g_emitters[e].mSpeed;
							g_buffers->phases[activeCount] = phase;

							g_buffers->activeIndices.push_back(activeCount);

							activeCount++;
						}
					}
				}
			}
		}
	}
}

void flexHelper::ErrorCallBack(NvFlexErrorSeverity, const char* msg, const char* file, int line)
{
	printf("Flex: %s - %s:%d\n", msg, file, line);
	g_Error = true;
	//assert(0); asserts are bad for TeamCity
}

struct Emitter {
	Emitter() : mSpeed(0.0f), mEnabled(false), mLeftOver(0.0f), mWidth(8) {}

	Vec3 mPos;
	Vec3 mDir;
	Vec3 mRight;
	float mSpeed;
	bool mEnabled;
	float mLeftOver;
	int mWidth;
};