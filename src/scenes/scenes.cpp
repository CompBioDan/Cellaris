#include <cmath>
#include <iostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iosfwd>
#include "scenes.h"

#include "../utilities/scenetime.h"
#include "../cells/cellpopulation.h"
#include "../utilities/dataexporter.h"


// pointer to single instance
Scene* Scene::mpInstance = nullptr;


// @return a pointer to the scene object
Scene* Scene::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new Scene;
		std::atexit(Destroy);
	}
	return mpInstance;
}

Scene::Scene() {}

double Scene::getDt()
{
	return mDt;
}

void Scene::setBirths(int births)
{
	mNumBirths = births;
}

unsigned Scene::getNumBirths()
{
	return mNumBirths;
}

void Scene::setDt(double dt)
{
	mDt = dt;
}

void Scene::setEndTime(double endTime)
{
	mEndTime = endTime;
}

void Scene::setOutputDirectory(std::string outputDirectory)
{
	mSceneOutputDirectory = outputDirectory;
}

void Scene::setSamplingTimeStepMultiple(unsigned samplingTimestepOutput)
{
	mSamplingTimestepMultiple = samplingTimestepOutput;
}

void Scene::Destroy()
{
	if (mpInstance)
	{
		delete mpInstance;
		mpInstance = nullptr;
	}
}

int Scene::getNumberCells()
{
	return cell_population.getCount();
}

int Scene::getNumberActiveParticles()
{
	return numActiveParticles;
}

void Scene::setNumberActiveParticles(int activeparticles)
{
	numActiveParticles = activeparticles;
}

void Scene::Solve()
{

	//// set up simulation time
	SceneTime* p_scene_time = SceneTime::Instance();
	double current_time = p_scene_time->GetTime();

	// set the end time and number of timesteps
	unsigned num_time_steps = (unsigned)((mEndTime - current_time) / mDt + 0.5);
	std::cout <<"tsteps "<< mDt << '\n';
	if (current_time > 0)
	{
		p_scene_time->ResetEndTimeAndNumberOfTimeSteps(mEndTime, num_time_steps);
	}
	else
	{
		p_scene_time->SetEndTimeAndNumberOfTimeSteps(mEndTime, num_time_steps);
	}

	// set the current time
	double time_now = p_scene_time->GetTime();

	// correctly age all cells before main solve loop
	for (int i = 0; i < getNumberCells(); ++i)
	{
		cell_population[i]->age();
	}
	std::cout << "end time: " << p_scene_time->GetEndTime() << '\n';
	std::cout << "current time: " << p_scene_time->GetTime() << '\n';
	std::cout << "at end? " << p_scene_time->HasFinished() << '\n';

	// Main solve loop
	while (!(p_scene_time->HasFinished()))
	{

		//* Output simulation population data with set frequency*/
		if ((p_scene_time->GetTimeStepsElapsed() % mSamplingTimestepMultiple)==0) {
			for (int i = 0; i < getNumberCells(); ++i)
			{
				//outputcellsdata(i,getCell(i));

				allCellDataWriter(getCell(i), "C:/Users/BCL/Documents/Cellaris/CellarisFlex/celldata.txt");
			}

		}

		//UpdateCellPopulation();
		for (int i = 0; i < getNumberCells(); ++i)
		{
			if (cell_population[i]->readyToDivide())
			{
				addCell(cell_population[i]->divide());
			}
		}

		p_scene_time->IncrementTimeOneStep();
	}


}

void Scene::addCell(Cell* cell)
{
	cell_population.add(cell);
}

Cell* Scene::getCell(unsigned int cell_id)
{
	if (cell_id >= cell_population.getSize())
		return NULL; 
	else 
		return cell_population[cell_id];
}


/** MOVED TO SEPERATE DATA OUTPUT CLASS*/
//void Scene::outputcellsdata(int cellID, Cell* cell)
//{
//	const char* g_outputFilename = "C:/Users/BCL/Documents/Cellaris/CellarisFlex/celldata.txt";
//	std::ofstream g_outputFile;
//
//	g_outputFile.open(g_outputFilename, std::ofstream::out | std::ofstream::app);
//	g_outputFile << std::fixed << std::setprecision(6);
//
//	SceneTime* p_scene_time = SceneTime::Instance();
//	g_outputFile << "Time: " << p_scene_time->GetTime() << std::endl;
//	g_outputFile << "CellID: " << cell->cell_id << std::endl;
//	g_outputFile << "Cellage: " << cell->cell_age << std::endl;
//	g_outputFile << "ParticleCount: " << cell->number_particles << std::endl;
//	g_outputFile << "ParticleOffset: " << cell->particle_offset << std::endl;
//	g_outputFile << "SpringOffset: " << cell->spring_offset << std::endl;
//	g_outputFile << "SpringCount: " << cell->number_springs << std::endl;
//	g_outputFile << "CellBirthTime: " << cell->cell_birth_time << std::endl;
//	g_outputFile << "CellPosition: " << cell->cell_position.x << std::endl;
//
//	g_outputFile.close();
//}

//allCellDataWriter* testWriter;