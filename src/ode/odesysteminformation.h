#ifndef _ODESYSTEMINFORMATION_H_
#define _ODESYSTEMINFORMATION_H_

#include <vector>
#include <string>
#include <map>

/**
* ODE system information class to allow access to information about a particular ODE system class 
*
* Information includes 
* - name for the system
* - names and units of free variables
* - name and units of state variables
* - names and units of settable parameters
* - name and units of derived quantities
*/

class odeSystemInformation
{

protected: 

	std::string systemName;

	std::string freeVariableName;

	std::string freeVariableUnits;

	std::vector<std::string> variableNames;

	std::vector<std::string> variableUnits;

	std::vector<std::string> parameterNames;

	std::vector<std::string> parameterUnits;

	std::vector<std::string> derivedQuantityNames;

	std::vector<std::string> derivedQuantityUnits;

	std::map<std::string, double> attributes;

	std::vector<double> initialConditions;

	bool initialised;

	virtual void Initialise() = 0;

public:

	odeSystemInformation();

	virtual ~odeSystemInformation();

	std::string getSystemName() const;

	std::string getFreeVariableName() const;

	std::string getFreeVariableUnits() const;

	void setDefaultInitialConditions(const std::vector<double>& rInitialConditions);

	void setDefaultInitialCondition(unsigned index, double initialCondition);

	std::vector<double> getIntiailConditions() const;

	cosnt std::vector<std::string>& rGetStateVariableNames() const;

	const std::vector<std::string>& rGetStateVariableUnits() const;

	unsigned getStateVariableIndex(const std::string& rName) const;

	bool hasStateVariable(const std::string& rName) const;

	std::string GetStateVariableUnits(unsigned index) const;

	const std::vector<std::string>& rGetParameterNames() const;


	const std::vector<std::string>& rGetParameterUnits() const;


	unsigned getParameterIndex(const std::string& rName) const;


	bool hasParameter(const std::string& rName) const;

	std::string getParameterUnits(unsigned index) const;


	unsigned getNumberOfParameters() const;


	unsigned getAnyVariableIndex(const std::string& rName) const;

	bool hasAnyVariable(const std::string& rName) const;


	std::string getAnyVariableUnits(unsigned index) const;


	const std::vector<std::string>& rGetDerivedQuantityNames() const;

	const std::vector<std::string>& rGetDerivedQuantityUnits() const;


	unsigned getDerivedQuantityIndex(const std::string& rName) const;


	bool hasDerivedQuantity(const std::string& rName) const;


	std::string getDerivedQuantityUnits(unsigned index) const;

	unsigned getNumberOfDerivedQuantities() const;

	//
	// Attribute methods
	//


	unsigned getNumberOfAttributes() const;


	bool hasAttribute(const std::string& rName) const;


	double getAttribute(const std::string& rName) const;







}

#endif /*_ODESYSTEMINFORMATION_H_*/