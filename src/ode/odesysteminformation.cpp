#include <cassert>
#include <algorithm>

#include "odesysteminformation.h

odeSystemInformation::odeSystemInformation()
	: initialised(false)
{
}

odeSystemInformation::~odeSystemInformation()
{
}

std::string odeSystemInformation::getSystemName() const
{
	return mSystemName;
}

std::string odeSystemInformation::getFreeVariableName() const
{
	return mFreeVariableName;
}

std::string odeSystemInformation::getFreeVariableUnits() const
{
	return mFreeVariableUnits;
}

void odeSystemInformation::setDefaultInitialConditions(const std::vector<double>& rInitialConditions)
{
	assert(initialised);
	initialConditions = rInitialConditions;
}

void odeSystemInformation::setDefaultInitialCondition(unsigned index, double initialCondition)
{
	assert(initialised);
	initialConditions.at(index) = initialCondition;
}

std::vector<double> odeSystemInformation::GetInitialConditions() const
{
	assert(mInitialised);
	return initialConditions;
}

const std::vector<std::string>& odeSystemInformation::rGetStateVariableNames() const
{
	assert(initialised);
	return variableNames;
}

const std::vector<std::string>& odeSystemInformation::rGetStateVariableUnits() const
{
	assert(initialised);
	return variableUnits;
}

unsigned odeSystemInformation::GetStateVariableIndex(const std::string& rName) const
{
	assert(initialised);
	std::vector<std::string>::const_iterator it = find(variableNames.begin(), variableNames.end(), rName);
	if (it == variableNames.end())
	{
		EXCEPTION("No state variable named '" + rName + "'.");
	}
	return (unsigned)(it - variableNames.begin());
}

bool odeSystemInformation::hasStateVariable(const std::string& rName) const
{
	assert(initialised);
	std::vector<std::string>::const_iterator it = find(variableNames.begin(), variableNames.end(), rName);
	return (it != variableNames.end());
}

std::string odeSystemInformation::getStateVariableUnits(unsigned index) const
{
	assert(initialised);
	if (index >= variableUnits.size())
	{
		EXCEPTION("The index passed in must be less than the number of state variables.");
	}
	return variableUnits[index];
}

const std::vector<std::string>& odeSystemInformation::rGetParameterNames() const
{
	assert(initialised);
	return parameterNames;
}

const std::vector<std::string>& odeSystemInformation::rGetParameterUnits() const
{
	assert(initialised);
	return parameterUnits;
}

unsigned odeSystemInformation::getParameterIndex(const std::string& rName) const
{
	assert(initialised);
	std::vector<std::string>::const_iterator it = find(parameterNames.begin(), parameterNames.end(), rName);
	if (it == parameterNames.end())
	{
		EXCEPTION("No parameter named '" + rName + "'.");
	}
	return (unsigned)(it - parameterNames.begin());
}

bool odeSystemInformation::hasParameter(const std::string& rName) const
{
	assert(initialised);
	std::vector<std::string>::const_iterator it = find(parameterNames.begin(), parameterNames.end(), rName);
	return (it != parameterNames.end());
}

std::string odeSystemInformation::getParameterUnits(unsigned index) const
{
	assert(initialised);
	if (index >= parameterUnits.size())
	{
		EXCEPTION("The index passed in must be less than the number of parameters.");
	}
	return parameterUnits[index];
}

unsigned odeSystemInformation::getNumberOfParameters() const
{
	assert(initialised);
	return parameterUnits.size();
}

unsigned odeSystemInformation::getAnyVariableIndex(const std::string& rName) const
{
	assert(initialised);
	if (hasStateVariable(rName))
	{
		return getStateVariableIndex(rName);
	}
	else if (hasParameter(rName))
	{
		return variableNames.size() + getParameterIndex(rName);
	}
	else if (hasDerivedQuantity(rName))
	{
		return variableNames.size() + parameterNames.size() + getDerivedQuantityIndex(rName);
	}
	else
	{
		EXCEPTION("No state variable, parameter, or derived quantity named '" + rName + "'.");
	}
}


bool odeSystemInformation::hasAnyVariable(const std::string& rName) const
{
	assert(initialised);
	return (hasStateVariable(rName) || hasParameter(rName) || hasDerivedQuantity(rName));
}

std::string odeSystemInformation::getAnyVariableUnits(unsigned index) const
{
	assert(initialised);
	if (index < variableUnits.size())
	{
		return variableUnits[index];
	}
	else
	{
		unsigned offset = variableUnits.size();
		if (index - offset < parameterUnits.size())
		{
			return parameterUnits[index - offset];
		}
		else
		{
			offset += parameterUnits.size();
			if (index - offset < derivedQuantityUnits.size())
			{
				return derivedQuantityUnits[index - offset];
			}
			else
			{
				EXCEPTION("Invalid index passed to GetAnyVariableUnits.");
			}
		}
	}
}


const std::vector<std::string>& odeSystemInformation::rGetDerivedQuantityNames() const
{
	assert(initialised);
	return derivedQuantityNames;
}

const std::vector<std::string>& odeSystemInformation::rGetDerivedQuantityUnits() const
{
	assert(initialised);
	return derivedQuantityUnits;
}

unsigned odeSystemInformation::getDerivedQuantityIndex(const std::string& rName) const
{
	assert(initialised);
	std::vector<std::string>::const_iterator it = find(derivedQuantityNames.begin(), derivedQuantityNames.end(), rName);
	if (it == derivedQuantityNames.end())
	{
		EXCEPTION("No derived quantity named '" + rName + "'.");
	}
	return (unsigned)(it - derivedQuantityNames.begin());
}

bool odeSystemInformation::hasDerivedQuantity(const std::string& rName) const
{
	assert(initialised);
	std::vector<std::string>::const_iterator it = find(derivedQuantityNames.begin(), derivedQuantityNames.end(), rName);
	return (it != derivedQuantityNames.end());
}

std::string odeSystemInformation::getDerivedQuantityUnits(unsigned index) const
{
	assert(initialised);
	if (index >= derivedQuantityUnits.size())
	{
		EXCEPTION("The index passed in must be less than the number of derived quantities.");
	}
	return derivedQuantityUnits[index];
}

unsigned odeSystemInformation::getNumberOfDerivedQuantities() const
{
	assert(initialised);
	return mDerivedQuantityUnits.size();
}

unsigned odeSystemInformation::getNumberOfAttributes() const
{
	assert(initialised);
	return attributes.size();
}

bool odeSystemInformation::hasAttribute(const std::string& rName) const
{
	assert(initialised);
	return (attributes.find(rName) != attributes.end());
}

double odeSystemInformation::getAttribute(const std::string& rName) const
{
	assert(initialised);
	std::map<std::string, double>::const_iterator it = attributes.find(rName);
	if (it == attributes.end())
	{
		EXCEPTION("No attribute '" + rName + "' found.");
	}
	return it->second;
}