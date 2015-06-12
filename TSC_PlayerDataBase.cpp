// TSC_PlayerDataBase.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdio.h>
#include "sqlite3.h"

using namespace std;

string processInput(string input);
sqlite3_stmt * queryDatabase(string query);
string getUIDFromInputMessage(string input);
string executeSetterQueryString(string query);

struct Data
{
	bool ready = false;
	string params = "";
	string result = "";
};

unordered_map<long int, Data> tickets;
mutex mtx;

sqlite3 *dataBase;

atomic<bool> worker_working(false);
long int id = 0; // global ticket id
long int cur_id = 0; // current ticket id

extern "C"
{
	__declspec (dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
}

void worker()
{
	while (worker_working = id > cur_id) // next ticket exists?
	{
		mtx.lock();
		Data ticket = tickets[++cur_id]; // copy ticket
		mtx.unlock();

		string input = ticket.params; // get input
		string output = processInput(input); // process input
		ticket.result = output; // prepare result
		ticket.ready = true; // notify about result

		mtx.lock();
		tickets[cur_id] = ticket; // copy back the result
		mtx.unlock();
	}
}


string executeGetterQueryInt(string query){
	sqlite3_stmt *res = queryDatabase(query);
	int result = 0;
	while (sqlite3_step(res) == SQLITE_ROW){
		result = (sqlite3_column_int(res, 0));
	}
	
	return std::to_string(result);;
}

string executeGetterQueryString(string query){
	sqlite3_stmt *res = queryDatabase(query);
	string result = "";
	while (sqlite3_step(res) == SQLITE_ROW){
		result = std::string(reinterpret_cast<const char*>(sqlite3_column_text(res, 0)));
	}
	
	return result;
}

sqlite3_stmt * queryDatabase(string query) {
	sqlite3_stmt *res;
	int rec_count = 0;
	//const char *errMSG;
	const char *tail;

	int error = sqlite3_open("tsc_database.db", &dataBase);
	if (error)
	{
		//
	}

	error = sqlite3_prepare_v2(dataBase,
		query.c_str(),
		1000, &res, &tail);

	if (error != SQLITE_OK)
	{
		//
	}
	return res;
}



string executeSetterQueryString(string query){
	sqlite3_stmt *res;
	int rec_count = 0;
	//const char *errMSG;
	const char *tail;

	int error = sqlite3_open("tsc_database.db", &dataBase);
	if (error)
	{
		return "fail to open db";
	}
	error = sqlite3_exec(dataBase,
		query.c_str(),
		0, 0, 0);

	return "success";
}


string addSingleQuotes(string s) {
	s.insert(0, "'");
	s.append("'");
	return s;
};

string getPlayerMoney(string playerUID) {
	string query = "SELECT Money FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryInt(query);
	return output;
}

string getPlayerUniform(string playerUID) {
	string query = "SELECT Uniform FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerVest(string playerUID) {
	string query = "SELECT Vest FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerHeadgear(string playerUID) {
	string query = "SELECT Headgear FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerPrimaryWeapon(string playerUID) {
	string query = "SELECT PrimaryWeapon FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerSecondaryWeapon(string playerUID) {
	string query = "SELECT SecondaryWeapon FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerMagazines(string playerUID) {
	string query = "SELECT Magazines FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerBackpack(string playerUID) {
	string query = "SELECT Backpack FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerUniformItems(string playerUID) {
	string query = "SELECT UniformItems FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerVestItems(string playerUID) {
	string query = "SELECT VestItems FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerBackpackItems(string playerUID) {
	string query = "SELECT BackpackItems FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}

string getPlayerHandgun(string playerUID) {
	string query = "SELECT Handgun FROM Players WHERE Uid=" + playerUID;
	string output = executeGetterQueryString(query);
	return output;
}


string setPlayerMoney(string input) {
	string UID = getUIDFromInputMessage(input);
	int moneyIndex = input.find(",") + 1;
	string money = input.substr(moneyIndex);
	string query = "UPDATE Players SET Money=" + money + " WHERE Uid=" + UID;
	string output = executeSetterQueryString(query);
	return output;
}

string setPlayerUniform(string input){
	string UID = getUIDFromInputMessage(input);
	int uniformIndex = input.find(",") + 1;
	string uniform = input.substr(uniformIndex);
	string query = "UPDATE Players SET Uniform=" + addSingleQuotes(uniform) + " WHERE Uid=" + UID;
	string output = executeSetterQueryString(query);
	return output;
}

string setPlayerVest(string input){
	string UID = getUIDFromInputMessage(input);
	int vestIndex = input.find(",") + 1;
	string vest = input.substr(vestIndex);
	string query = "UPDATE Players SET Vest=" + addSingleQuotes(vest) + " WHERE Uid=" + UID;
	string output = executeSetterQueryString(query);
	return output;
}

string setPlayerBackpack(string input){
	string UID = getUIDFromInputMessage(input);
	int backpackIndex = input.find(",") + 1;
	string backpack = input.substr(backpackIndex);
	string query = "UPDATE Players SET Backpack=" + addSingleQuotes(backpack) + " WHERE Uid=" + UID;
	string output = executeSetterQueryString(query);
	return output;
}

string setPlayerHeadgear(string input){
	string UID = getUIDFromInputMessage(input);
	int headgearIndex = input.find(",") + 1;
	string headgear = input.substr(headgearIndex);
	string query = "UPDATE Players SET Headgear=" + addSingleQuotes(headgear) + " WHERE Uid=" + UID;
	string output = executeSetterQueryString(query);
	return output;
}

string setPlayerPrimaryWeapon(string input){
	string UID = getUIDFromInputMessage(input);
	int primaryWeaponIndex = input.find(",") + 1;
	string primaryWeapon = input.substr(primaryWeaponIndex);
	string query = "UPDATE Players SET PrimaryWeapon=" + addSingleQuotes(primaryWeapon) + " WHERE Uid=" + UID;
	string output = executeSetterQueryString(query);
	return output;
}

string setPlayerSecondaryWeapon(string input){
	string UID = getUIDFromInputMessage(input);
	int secondaryWeaponIndex = input.find(",") + 1;
	string secondaryWeapon = input.substr(secondaryWeaponIndex);
	string query = "UPDATE Players SET SecondaryWeapon=" + addSingleQuotes(secondaryWeapon) + " WHERE Uid=" + UID;
	string output = executeSetterQueryString(query);
	return output;
}

string setPlayerHandgun(string input){
	string UID = getUIDFromInputMessage(input);
	int handgunIndex = input.find(",") + 1;
	string handgun = input.substr(handgunIndex);
	string query = "UPDATE Players SET Handgun=" + addSingleQuotes(handgun) + " WHERE Uid=" + UID;
	string output = executeSetterQueryString(query);
	return output;
}

//Must be scrubbed of the command (s:A, r:B, etc . . .)
string getUIDFromInputMessage(string input) {
	int index = input.find(",");
	return input.substr(0, index);
}


bool isPlayerInDatabase(string playerUID) {
	string query = "SELECT TOP 1 Uid FROM Players WHERE Uid =" + playerUID;
	string result = executeGetterQueryString(query);
	if (!result.compare("")) {
		return false;
	}
	else {
		return true;
	}
}

string processInput(string input) {
	string command = input.substr(2, 1);
	string output = command;
	string inputClean = input.substr(3);

	if (!command.compare("A")) {
		output = getPlayerMoney(inputClean);
	}
	else if (!command.compare("B")) {
		output = getPlayerUniform(inputClean);
	}
	else if (!command.compare("C")) {
		output = getPlayerVest(inputClean);
	}
	else if (!command.compare("D")) {
		output = getPlayerHeadgear(inputClean);
	}
	else if (!command.compare("E")) {
		output = getPlayerPrimaryWeapon(inputClean);
	}
	else if (!command.compare("F")) {
		output = getPlayerSecondaryWeapon(inputClean);
	}
	else if (!command.compare("G")) {
		output = getPlayerMagazines(inputClean);
	}
	else if (!command.compare("H")) {
		output = getPlayerBackpack(inputClean);
	}
	else if (!command.compare("I")) {
		output = getPlayerUniformItems(inputClean);
	}
	else if (!command.compare("J")) {
		output = getPlayerVestItems(inputClean);
	}
	else if (!command.compare("K")) {
		output = getPlayerBackpackItems(inputClean);
	}
	else if (!command.compare("L")) {
		output = setPlayerMoney(inputClean);
	}
	else if (!command.compare("M")) {
		output = setPlayerUniform(inputClean);
	}
	else if (!command.compare("N")) {
		output = setPlayerVest(inputClean);
	}
	else if (!command.compare("P")) {
		output = setPlayerHeadgear(inputClean);
	}
	else if (!command.compare("Q")) {
		output = setPlayerBackpack(inputClean);
	}
	else if (!command.compare("R")) {
		output = setPlayerPrimaryWeapon(inputClean);
	}
	else if (!command.compare("S")) {
		output = setPlayerSecondaryWeapon(inputClean);
	}
	else if (!command.compare("T")) {
		output = setPlayerHandgun(inputClean);
	}

	return output;
}


void __stdcall RVExtension(char *output, int outputSize, const char *function)
{
	if (!strncmp(function, "r:", 2)) // detect checking for result
	{
		long int num = atol(&function[2]); // ticket number or 0

		if (tickets.find(num) != tickets.end()) // ticket exists
		{
			mtx.lock();
			if (tickets[num].ready) // result is ready
			{
				strncpy_s(output, outputSize, tickets[num].result.c_str(), _TRUNCATE); // result
				tickets.erase(num); // get rid of the read ticket
				mtx.unlock();
				return;
			}
			mtx.unlock();

			strncpy_s(output, outputSize, "WAIT", _TRUNCATE); // result is not ready
			return;
		}
		strncpy_s(output, outputSize, "EMPTY", _TRUNCATE); // no such ticket
	}
	else if (!strncmp(function, "s:", 2)) // detect ticket submission
	{
		Data data;
		data.params = string(&function[2]); // extract params

		mtx.lock();
		tickets.insert(pair<long int, Data>(++id, data)); // add ticket to the queue
		mtx.unlock();

		if (!worker_working) // if worker thread is finished, start another one
		{
			worker_working = true;
			thread worker(worker);
			worker.detach(); // start parallel process
		}
		strncpy_s(output, outputSize, to_string(id).c_str(), _TRUNCATE); // ticket number
	}
	else
	{
		strncpy_s(output, outputSize, "INVALID COMMAND", _TRUNCATE); // other input
	}
}