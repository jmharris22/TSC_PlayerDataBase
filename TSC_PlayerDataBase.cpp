// TSC_PlayerDataBase.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

struct Data
{
	bool ready = false;
	string params = "";
	string result = "";
};

unordered_map<long int, Data> tickets;
mutex mtx;

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
		string output = "output: " + input; // process input
		ticket.result = output; // prepare result
		ticket.ready = true; // notify about result

		mtx.lock();
		tickets[cur_id] = ticket; // copy back the result
		mtx.unlock();
	}
}



string getPlayerMoney(string playerUID) {

}

string getPlayerUniform(string playerUID) {

}

string getPlayerVest(string playerUID) {

}

string getPlayerHeadgear(string playerUID) {

}

string getPlayerPrimaryWeapon(string playerUID) {

}

string getPlayerSecondaryWeapon(string playerUID) {

}

string getPlayerMagazines(string playerUID) {

}

string getPlayerBackpack(string playerUID) {

}

string getPlayerUniformItems(string playerUID) {

}

string getPlayerVestItems(string playerUID) {

}

string getPlayerBackpackItems(string playerUID) {

}

bool isPlayerInDatabase(string playerUID) {

}

void processInput(string input) {
	string command = input.substr(0, 1);
	string output = "EMPTY";
	if (!command.compare("A")) {
		output = getPlayerMoney(input.substr(2));
	}
	else if (!command.compare("B")) {

	}
	else if (!command.compare("C")) {

	}
	else if (!command.compare("D")) {

	}
	else if (!command.compare("E")) {

	}
	else if (!command.compare("F")) {

	}
	else if (!command.compare("G")) {

	}
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

