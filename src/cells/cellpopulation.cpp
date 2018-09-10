
#include "cellpopulation.h"


CellPopulation::CellPopulation(std::vector<Cell*>& rCells)
	: all_cells(rCells.begin(), rCells.end())
{
	std::vector<Cell*>().swap(rCells);
}

void CellPopulation::InitialiseCells()
{
	// Can set up the initializing of cell-cycle, cell positions etc here
}

std::list<Cell*>& CellPopulation::get_cells()
{
	return all_cells;
}

unsigned CellPopulation::get_number_all_cells()
{
	return all_cells.size();
}



