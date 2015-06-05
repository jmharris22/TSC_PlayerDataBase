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
sqlite3_stmt * queryDaabase(string query);

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
	sqlite3_stmt *res = queryDaabase(query);
	int result = 0;
	while (sqlite3_step(res) == SQLITE_ROW){
		result = (sqlite3_column_int(res, 0));
	}
	
	return std::to_string(result);;
}

string executeGetterQueryString(string query){
	sqlite3_stmt *res = queryDaabase(query);
	string result = "";
	while (sqlite3_step(res) == SQLITE_ROW){
		result = std::string(reinterpret_cast<const char*>(sqlite3_column_text(res, 0)));
	}
	
	return result;
}

sqlite3_stmt * queryDaabase(string query) {
	sqlite3_stmt *res;
	int rec_count = 0;
	const char *errMSG;
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

bool isPlayerInDatabase(string playerUID) {

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