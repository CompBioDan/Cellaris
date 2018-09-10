#include "dataexporter.h"
#include <iostream>
#include <iomanip>
#include <iosfwd>
#include <fstream>
#include "../cells/cellpopulation.h"
#include "../cells/cell.h"

class Cell;

/** Output all cell held data. Input: Pointer to cell & output filename */
void allCellDataWriter(Cell* cell, const char* outputFilename)
{
	const char* g_outputFilename = outputFilename;
	std::ofstream g_outputFile;

	g_outputFile.open(g_outputFilename, std::ofstream::out | std::ofstream::app);
	g_outputFile << std::fixed << std::setprecision(6);

	SceneTime* p_scene_time = SceneTime::Instance();
	g_outputFile << "Time: " << p_scene_time->GetTime() << std::endl;
	g_outputFile << "CellID: " << cell->cell_id << std::endl;
	g_outputFile << "Cellage: " << cell->cell_age << std::endl;
	g_outputFile << "ParticleCount: " << cell->number_particles << std::endl;
	g_outputFile << "ParticleOffset: " << cell->particle_offset << std::endl;
	g_outputFile << "SpringOffset: " << cell->spring_offset << std::endl;
	g_outputFile << "SpringCount: " << cell->number_springs << std::endl;
	g_outputFile << "CellBirthTime: " << cell->cell_birth_time << std::endl;
	g_outputFile << "CellPosition: " << cell->cell_position.x << std::endl;

	g_outputFile.close();

}

/** Output cell age data.  Input: Pointer to cell & output filename **/
void cellAgeWriter(Cell* cell, const char* outputFilename)
{
	const char* g_outputFilename = outputFilename;
	std::ofstream g_outputFile;

	g_outputFile.open(g_outputFilename, std::ofstream::out | std::ofstream::app);
	g_outputFile << std::fixed << std::setprecision(6);

	SceneTime* p_scene_time = SceneTime::Instance();
	g_outputFile << "Time: " << p_scene_time->GetTime() << std::endl;
	g_outputFile << "CellID: " << cell->cell_id << std::endl;
	g_outputFile << "Cellage: " << cell->cell_age << std::endl;
	

	g_outputFile.close();

}
