#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <queue>
using namespace std;

struct reservation_station {
	bool Busy;
	string Op;
	short Vj;
	short Vk;
	pair<string, short> Qj;
	pair<string, short> Qk;
	short A;
	short exe_cycles;
	short counter;
	string station_name;
	short station_num;
	short inst_num;
	short stage;
	bool branch_predict;
	short table_pos;

};

vector<short> parse(string input)
{
	vector<short> out;
	
	string op, rd, rs1, rs2;
	for (short i = 0; i < input.size(); i++)
	{
		if (input[i] == ' ')
		{
			op = input.substr(0, i);
			input.erase(0, i+1);
			
			break;
		}
		else if (i == input.size() - 1)
		{
			op = input.substr(0, i + 1);
			input.erase(0, i + 1);
			break;
		}
		
	}
	for (short i = 0; i < input.size(); i++)
	{
		if (input[i] == ',')
		{
			rd = input.substr(0, i);
			input.erase(0, i+2);
			break;
		}
		else if (i == input.size() - 1)
		{
			rd = input.substr(0, i+1);
			input.erase(0, i+1);
			break;
		}
	}
	for (short i = 0; i < input.size(); i++)
	{

		
		
		if (input[i] == ',' || input[i] == '(')
		{
			rs1 = input.substr(0, i);
			if (input[i] == ',') i++;
			input.erase(0, i+1);
			break;
		}
		else if (i == input.size() - 1)
		{
			rs1 = input.substr(0, i + 1);
			input.erase(0, i + 1);
			break;
		}
	}
	for (short i = 0; i < input.size(); i++)
	{
		if (input[i] == ')')
		{
			rs2 = input.substr(0, i);
			input.erase(0, i+1 );
			break;
		}
		else if (i == input.size() - 1)
		{
			rs2 = input.substr(0, i+1);
			input.erase(0, i+2);
			break;

		}
	}

	if (!rd.empty()&&rd[1] == '\0')rd[1] = '0';
	if (!rs1.empty()&&rs1[1] == '\0')rs1[1] = '0';
	if (!rs2.empty()&&rs2[1] == '\0')rs2[1] = '0';

	if (op == "LW")
	{
		out.push_back(1);
		out.push_back(rd[1] - '0');
		out.push_back(atoi(rs1.c_str()));

		out.push_back(rs2[1] - '0');
	}
	else if (op == "SW")
	{
		out.push_back(2);
		out.push_back(atoi(rs1.c_str()));
		out.push_back(rs2[1] - '0');
		out.push_back(rd[1] - '0');
		
		
	}
	else if (op == "BEQ")
	{
		out.push_back(3);
		out.push_back(rd[1] - '0');
		
		out.push_back(rs1[1] - '0');
		out.push_back(atoi(rs2.c_str())/2);
	}
	else if (op == "JALR")
	{
		out.push_back(4);
		out.push_back(0);
		out.push_back(0);
		out.push_back(rd[1] - '0');
	}
	else if (op == "RET")
	{
		out.push_back(5);
		out.push_back(0);
		out.push_back(0);
		out.push_back(0);
	}
	else if (op == "ADD")
	{
		out.push_back(6);
		out.push_back(rd[1] - '0');
		out.push_back(rs1[1] - '0');
		out.push_back(rs2[1] - '0');
	}
	else if (op == "NEG")
	{
		out.push_back(7);
		out.push_back(rd[1] - '0');
		out.push_back(rs1[1] - '0');
		out.push_back(0);
	}
	else if (op == "ADDI")
	{
		out.push_back(8);
		out.push_back(rd[1] - '0');
		out.push_back(rs1[1] - '0');
		out.push_back(atoi(rs2.c_str()));
	}
	else if (op == "DIV")
	{
		out.push_back(9);
		out.push_back(rd[1] - '0');
		out.push_back(rs1[1] - '0');
		out.push_back(rs2[1] - '0');
	}
	else cout << "\nInvalid Input!\n";
	return out;
}

//Busy Op Vj Vk Qj Qk A[2]
vector<vector<reservation_station>> All_RSs(6);

// vector<reservation_station> LW_units;
// vector<reservation_station> SW_units;
// vector<reservation_station> BEQ_units;
// vector<reservation_station> JALR_RET_units;
// vector<reservation_station> ADD_NEG_ADDI_units;
// vector<reservation_station> DIV_units;

short check_available_RS(vector<reservation_station> RSs) {
	for (short i = 0; i < RSs.size(); i++) {
		if (RSs[i].Busy == 0) {
			return i;
		}
	}
	return -1;
}

vector<reservation_station>& OP_CODE_ASSOCIATED_RSs(short inst) {
	// LW --> 1
	// SW --> 2
	// BEQ --> 3
	// JALR --> 4
	// RET --> 5
	// ADD --> 6
	// NEG --> 7
	// ADDI --> 8
	// DIV --> 9
	switch (inst) {
	case 1:return All_RSs[0];//LW_units; 
	case 2:return All_RSs[1];//SW_units;
	case 3:return All_RSs[2];//BEQ_units;
	case 4:
	case 5:return All_RSs[3];//JALR_RET_units;
	case 6:
	case 7:
	case 8:return All_RSs[4];//ADD_NEG_ADDI_units;
	case 9:return All_RSs[5];//DIV_units;
	}
}

short main() {
	short pc_start;
	vector<string> instrucitons;
	vector<vector<short>> instrucitons_vals;
	cout << "Input the address of the first instruction\n";
	cin >> pc_start;
	cout << "Input the number of instructions\n";
	short input_counter;
	cin >> input_counter;
	cin.clear();
	cin.ignore(1000, '\n');
	cout << "Input all the instructions seperated by new lines\n";
	
	while (input_counter > 0)
	{
		string i_s;
		getline(cin, i_s);
		instrucitons.push_back(i_s);
		instrucitons_vals.push_back(parse(i_s));
		input_counter--;
	}
	vector<short> mem(64000, 0);
	cout << "Input the number of data memory items\n";
	cin >> input_counter;
	cout << "Input the address of each item in the memory followed by its value\n";
	while (input_counter > 0)
	{
		short m, n;
		cin >> m >> n;
		mem[m] = n;
		input_counter--;
	}

	for (short i = 0; i < 2; i++) {
		reservation_station rs1 = { 0,"NOP",0,-1,{"0",-1},{"0",-1},0,2,0,"LW_unit",i,0,0,0 };
		reservation_station rs2 = { 0,"NOP",0,0,{"0",-1},{"0",-1},0,2,0,"SW_unit",i,0,0,0 };
		reservation_station rs3 = { 0,"NOP",0,0,{"0",-1},{"0",-1},0,1,0,"BEQ_unit",i,0,0 ,0};
		reservation_station rs4 = { 0,"NOP",0,0,{"0",-1},{"0",-1},0,1,0,"JALR_RET_unit",i,0,0,0 };
		reservation_station rs5 = { 0,"NOP",0,0,{"0",-1},{"0",-1},-1,2,0,"ADD_NEG_ADDI_unit",i,0,0,0 };
		reservation_station rs6 = { 0,"NOP",0,0,{"0",-1},{"0",-1},-1,8,0,"DIV_unit",i,0,0,0 };
		if (i < 2)All_RSs[0].push_back(rs1);
		if (i < 2)All_RSs[1].push_back(rs2);
		if (i < 1)All_RSs[2].push_back(rs3);
		if (i < 1)All_RSs[3].push_back(rs4);
		if (i < 2)All_RSs[4].push_back(rs5);
		if (i < 1)All_RSs[5].push_back(rs6);
	}

	vector<vector<short>> table(0, vector<short>(4, 0));

	vector<pair<string, short>> registers_ready_or_not(1000, { "0",0 }); // assuming all registers are ready at the beginning
	vector<short> registers(32, 0); // assuming all registers are ready at the beginning

	vector<std::unique_ptr<reservation_station>> inst_Rs(instrucitons_vals.size());
	vector<pair<reservation_station*, bool>> write_queue;
	short pc = 200;
	bool br, jmp;
	br = jmp = false;
	bool not_complete = true;
	short i = 0;
	short clock_cycle = 1;
	short return_position;
	short branch_miss=0;
	short branch_predict = 0;
	queue<reservation_station*> load_store_queue;
	while (not_complete && clock_cycle < 200) {
		//issuing --> one issue at a time
		//check whether RS is free or not

		//????? add table entry
		//????? do not issue if there is a jump instructino executing
		if (i < instrucitons_vals.size() && !jmp) { // ?????? if current pc greater than last pc
			vector<reservation_station> & RSs = OP_CODE_ASSOCIATED_RSs(instrucitons_vals[i][0]);
			short RS_num = check_available_RS(RSs);
			if (RS_num != -1) {
				//check the readiness of each source register
				//rs1
				if (RSs[RS_num].Qj.first != "-1" && RSs[RS_num].station_name!="LW_unit" && RSs[RS_num].station_name != "JALR_RET_unit"  && registers_ready_or_not[instrucitons_vals[i][2]].first != "0") { // if not equal to 0 --> not ready
					RSs[RS_num].Qj = make_pair(registers_ready_or_not[instrucitons_vals[i][2]].first, registers_ready_or_not[instrucitons_vals[i][2]].second); // fill the Qk field of the current RS with the name of the reservation station
				}
				else 
				{	
					if (RSs[RS_num].station_name != "LW_unit" )
					{
						RSs[RS_num].Vj = registers[instrucitons_vals[i][2]];
						RSs[RS_num].Qj = { "0",0 };
					}
					else RSs[RS_num].Qj = { "0",0 };
				}
				//rs2
				if (RSs[RS_num].Qk.first != "-1"  &&RSs[RS_num].station_name != "BEQ_unit"&&instrucitons_vals[i][0] != 5&&instrucitons_vals[i][0]!=8&& registers_ready_or_not[instrucitons_vals[i][3]].first != "0") { // if not equal to 0 --> not ready
					RSs[RS_num].Qk = make_pair(registers_ready_or_not[instrucitons_vals[i][3]].first, registers_ready_or_not[instrucitons_vals[i][3]].second); // fill the Qk field of the current RS with the name of the reservation station
				}
				else {
					if (RSs[RS_num].station_name != "BEQ_unit"&&instrucitons_vals[i][0] != 5 && instrucitons_vals[i][0] != 8)
					{
						RSs[RS_num].Vk = registers[instrucitons_vals[i][3]];
						RSs[RS_num].Qk = { "0",0 };
					}
					else RSs[RS_num].Qk = { "0",0 };
				}

				if (RSs[RS_num].station_name=="LW_unit") { // in case of SW LW JUMP Branch
					RSs[RS_num].A = instrucitons_vals[i][2];
				}
				if (RSs[RS_num].station_name == "SW_unit")
				{
					RSs[RS_num].A = instrucitons_vals[i][1];
				}
				if (instrucitons_vals[i][0] == 4 || instrucitons_vals[i][0] == 5) //if jumping
				{
					jmp = true;
				}

				if (instrucitons_vals[i][0] == 3) //if branching 
				{
					br = true;
				}

				RSs[RS_num].Busy = true;
				RSs[RS_num].stage = 1;
				RSs[RS_num].inst_num = i;
				RSs[RS_num].branch_predict = (br&&RSs[RS_num].station_name!= "BEQ_unit");
				table.push_back({ clock_cycle,0,0,i });
				RSs[RS_num].table_pos = table.size() - 1;
				// inst_Rs[i]=std::make_unique<reservation_station>(*RSs[RS_num]);
				registers_ready_or_not[instrucitons_vals[i][1]].first = RSs[RS_num].station_name;
				registers_ready_or_not[instrucitons_vals[i][1]].second = RSs[RS_num].station_num;
				write_queue.push_back(make_pair(&RSs[RS_num], 0));
				if (RSs[RS_num].station_name == "LW_unit" || RSs[RS_num].station_name == "SW_unit") load_store_queue.push(&RSs[RS_num]);
				if(!jmp) i++;
			}
		}


		for (short j = 0; j < All_RSs.size(); j++) {
			for (short k = 0; k < All_RSs[j].size(); k++) {

				short inst = All_RSs[j][k].inst_num;

				if (All_RSs[j][k].stage == 1 && All_RSs[j][k].Busy == true ) {//

					if (instrucitons_vals[inst][0] == 1  && (&All_RSs[j][k]==load_store_queue.front()) ) {

						if (All_RSs[j][k].Qj.first == "0") {  //???? load and store in order (handled using a queue of issued RSs in order)
							// cout<<clock_cycle<<endl;
							All_RSs[j][k].stage = 2;
							All_RSs[j][k].counter = All_RSs[j][k].exe_cycles;
							All_RSs[j][k].A += All_RSs[j][k].Vj;
						}

					}
					else if (instrucitons_vals[inst][0] == 2 && (&All_RSs[j][k] == load_store_queue.front())) {

						if (All_RSs[j][k].Qj.first == "0" && All_RSs[j][k].Qk.first == "0") {  //???? load and store in order (handled using a queue of issued RSs in order)
							// cout<<clock_cycle<<endl;
							All_RSs[j][k].stage = 2;
							All_RSs[j][k].counter = All_RSs[j][k].exe_cycles;
							All_RSs[j][k].A += All_RSs[j][k].Vj;
						}

					}
					else if (All_RSs[j][k].Qj.first == "0" && All_RSs[j][k].Qk.first == "0" &&!(instrucitons_vals[inst][0] == 1 || instrucitons_vals[inst][0] == 2)) {
						All_RSs[j][k].stage = 2;
						All_RSs[j][k].counter = All_RSs[j][k].exe_cycles;
					}

				}
				else if (All_RSs[j][k].stage == 2 && All_RSs[j][k].counter > 1) {
					/*
					 8 -8
					 9 -7
					 10-6
					 11-5
					 12-4
					 13-3
					 14-1
					 15-0
					*/
					All_RSs[j][k].counter--;
				}
				else if (All_RSs[j][k].stage == 2 && All_RSs[j][k].counter == 1 && !All_RSs[j][k].branch_predict) { // separate writing from exe ????
					//excute instruction

					//handling two WB at the same time ?????
					// vector of issued instructions
					// when finished execution, set flag  to true
					// when writing, go ascendingely
					// when first flag==true is found
					// write the value from the respective reservation station
					// and delete entry

					// cout<<inst<<" "<<clock_cycle<<endl;
					// if(table[inst][1]==0)
					table[All_RSs[j][k].table_pos][1] = clock_cycle;
					All_RSs[j][k].counter--;
					

					// LW --> 1
				// SW --> 2
				// BEQ --> 3
				// JALR --> 4
				// RET --> 5
				// ADD --> 6
				// NEG --> 7
				// ADDI --> 8
				// DIV --> 9


				}
				else if (All_RSs[j][k].stage == 2 && All_RSs[j][k].counter == 0) {
					for (short wq = 0; wq < write_queue.size(); wq++)         //marks reservation station as ready for writing
					{
						// if(clock_cycle==4)cout<<(write_queue[wq].first)->station_name<<" "<<All_RSs[j][k].station_name<<endl;
						if (write_queue[wq].first == &All_RSs[j][k])
						{
							write_queue[wq].second = true;
							// cout<<"entereeeeeeeeeeeeeeeeeeeeeeeeed\n";
							break;
						}
						All_RSs[j][k].stage = 3;
					}

				}
			}

		}

		// writing begins hereeeeeeeee
		for (short wq = 0; wq < write_queue.size(); wq++) {
			short valu;
			if (write_queue[wq].second)
			{

				//if jump or branch, update PC and fix flags. Also if branch, either flush or set the branch_predict flag to false for all reservation stations after it in the write_queue
				//if arithemetic or load, broadcast
				//if  store, update memory
				write_queue[wq].first->stage = 0;
				short inst = write_queue[wq].first->inst_num;
				write_queue[wq].first->Busy = false;
				table[write_queue[wq].first->table_pos][2] = clock_cycle;
				if ((registers_ready_or_not[instrucitons_vals[inst][1]].first == write_queue[wq].first->station_name) && (registers_ready_or_not[instrucitons_vals[inst][1]].second == write_queue[wq].first->station_num)) {
					registers_ready_or_not[instrucitons_vals[inst][1]].first = "0";
					registers_ready_or_not[instrucitons_vals[inst][1]].second = 0;
				}
				switch (instrucitons_vals[inst][0]) {
				case 1:
					if (instrucitons_vals[inst][1] != 0)
					{
						valu = registers[instrucitons_vals[inst][1]] = mem[write_queue[wq].first->A];
					}
					else {
						valu = registers[instrucitons_vals[inst][1]] = 0;
					}
					load_store_queue.pop();
					if (!load_store_queue.empty() && load_store_queue.front()->Qj.first == "0" && load_store_queue.front()->station_name == "LW_unit")
					{
						load_store_queue.front()->stage = 2;
						load_store_queue.front()->counter = load_store_queue.front()->exe_cycles;
						load_store_queue.front()->A += load_store_queue.front()->Vj;
					}
					else if (!load_store_queue.empty() && load_store_queue.front()->Qk.first == "0" && load_store_queue.front()->Qj.first == "0" && load_store_queue.front()->station_name == "LW_unit")
					{
						load_store_queue.front()->stage = 2;
						load_store_queue.front()->counter = load_store_queue.front()->exe_cycles;
						load_store_queue.front()->A += load_store_queue.front()->Vj;
					}
					break;
				case 2:
					mem[write_queue[wq].first->A] = registers[instrucitons_vals[inst][3]];
					load_store_queue.pop();
					if (!load_store_queue.empty() && load_store_queue.front()->Qj.first == "0" && load_store_queue.front()->station_name=="LW_unit")
					{
						load_store_queue.front()->stage = 2;
						load_store_queue.front()->counter = load_store_queue.front()->exe_cycles;
						load_store_queue.front()->A += load_store_queue.front()->Vj;
					}else if (!load_store_queue.empty() && load_store_queue.front()->Qk.first == "0" && load_store_queue.front()->Qj.first == "0" && load_store_queue.front()->station_name == "LW_unit")
					{
						load_store_queue.front()->stage = 2;
						load_store_queue.front()->counter = load_store_queue.front()->exe_cycles;
						load_store_queue.front()->A += load_store_queue.front()->Vj;
					}
					break;
				case 3:
					//cout << "case beq";
					br = false;
					if (registers[instrucitons_vals[inst][1]] == registers[instrucitons_vals[inst][2]]) {
						//cout << "branch taken\n";
						branch_miss++;
						for (short wqq = wq+1; wqq < write_queue.size(); wqq++)
						{


							short wqq_inst = write_queue[wqq].first->inst_num;
							write_queue[wqq].first->Busy = false;
							write_queue[wqq].first->stage = 0;
							if ((registers_ready_or_not[instrucitons_vals[wqq_inst][1]].first == write_queue[wqq].first->station_name) && (registers_ready_or_not[instrucitons_vals[wqq_inst][1]].second == write_queue[wqq].first->station_num)) {
								registers_ready_or_not[instrucitons_vals[wqq_inst][1]].first = "0";
								registers_ready_or_not[instrucitons_vals[wqq_inst][1]].second = 0;
							}
							
							//write_queue[wqq].first->counter = 0;
							//write_queue[wqq].first->branch_predict = false;

						}
						i = inst + instrucitons_vals[inst][3];
					
					}
					else {
						//cout << "branch not taken\n";
						branch_predict++;
						for (short wqq = wq+1; wqq < write_queue.size(); wqq++)
							write_queue[wqq].first->branch_predict = false;
						
					}
					break;
				case 4:
					return_position = i+1 ;
					i = (registers[instrucitons_vals[inst][3]]-pc_start)/2;
					//cout << registers[instrucitons_vals[inst][3]];
					//cout << endl << i << endl;
					jmp = false;
					break;
				case 5:
					i = return_position;
					jmp = false;
					break;
				case 6:
					// cout<<"look:"<<registers[instrucitons_vals[inst][3]]<<endl;
					if (instrucitons_vals[inst][1] != 0)
					{
						valu = registers[instrucitons_vals[inst][1]] = registers[instrucitons_vals[inst][2]] + registers[instrucitons_vals[inst][3]];
					}
					else valu = registers[instrucitons_vals[inst][1]] = 0;
					 break;
				case 7:
					if (instrucitons_vals[inst][1] != 0)
					{
					valu = registers[instrucitons_vals[inst][1]] = -registers[instrucitons_vals[inst][2]];
					}
					else valu = registers[instrucitons_vals[inst][1]] = 0;
					break;
				case 8:
					if (instrucitons_vals[inst][1] != 0)
					{
					valu = registers[instrucitons_vals[inst][1]] = registers[instrucitons_vals[inst][2]] + instrucitons_vals[inst][3];
					}
					else valu = registers[instrucitons_vals[inst][1]] = 0;
					break;
				case 9:
					if (instrucitons_vals[inst][1] != 0)
					{
					if (registers[instrucitons_vals[inst][3]] != 0) valu = registers[instrucitons_vals[inst][1]] = registers[instrucitons_vals[inst][2]] / registers[instrucitons_vals[inst][3]];
					}
					else valu = registers[instrucitons_vals[inst][1]] = 0;
					break;
				}

				for (short j = 0; j < All_RSs.size(); j++) {
					for (short k = 0; k < All_RSs[j].size(); k++) {
						if (write_queue[wq].first->station_name != "BEQ_unit"&&write_queue[wq].first->station_name != "SW_unit" && (All_RSs[j][k].Qj.first == write_queue[wq].first->station_name) && (All_RSs[j][k].Qj.second == (write_queue[wq].first)->station_num)) {
							All_RSs[j][k].Qj.first = "0";
							All_RSs[j][k].Vj = valu;
							
							if (instrucitons_vals[inst][0] == 1 || instrucitons_vals[inst][0] == 2) {

								if (All_RSs[j][k].Qj.first == "0") {  //???? load and store in order (handled using a queue of issued RSs in order)
									// cout<<clock_cycle<<endl;
									All_RSs[j][k].stage = 2;
									All_RSs[j][k].counter = All_RSs[j][k].exe_cycles;
									All_RSs[j][k].A += All_RSs[j][k].Vj;
								}

							}
							else if (All_RSs[j][k].Qj.first == "0" && All_RSs[j][k].Qk.first == "0") {
								All_RSs[j][k].stage = 2;
								All_RSs[j][k].counter = All_RSs[j][k].exe_cycles;
							}
						}

						if (write_queue[wq].first->station_name != "BEQ_unit"&&write_queue[wq].first->station_name!="SW_unit"&&(All_RSs[j][k].Qk.first == write_queue[wq].first->station_name )&& (All_RSs[j][k].Qk.second == (write_queue[wq].first)->station_num)) {
							All_RSs[j][k].Qk.first = "0";
							All_RSs[j][k].Vk = valu;
							
							if (instrucitons_vals[inst][0] == 1 || instrucitons_vals[inst][0] == 2) {

								if (All_RSs[j][k].Qj.first == "0") {  //???? load and store in order (handled using a queue of issued RSs in order)
									// cout<<clock_cycle<<endl;
									All_RSs[j][k].stage = 2;
									All_RSs[j][k].counter = All_RSs[j][k].exe_cycles;
									All_RSs[j][k].A += All_RSs[j][k].Vj;
								}

							}
							else if (All_RSs[j][k].Qj.first == "0" && All_RSs[j][k].Qk.first == "0") {
								All_RSs[j][k].stage = 2;
								All_RSs[j][k].counter = All_RSs[j][k].exe_cycles;
							}
						}
					
					}
				}

				write_queue.erase(write_queue.begin() + wq);
				break;
			}
		}

		bool complete = true;
		for (short r = 0; r < table.size(); r++) { //?????
			if (table[r][2] == 0&& table[r][1] >0) {
				// cout<<r<<endl;
				complete = false;
			}
		}
		
		not_complete = !complete||(i<=instrucitons_vals.size());
		//advance i
		clock_cycle++;
	}

	int max_clk = 0;
	cout << "\n############################################\nResults:\n############################################\n";
	for (short i = 0; i < table.size(); i++)
	{
		
		if (table[i][2] == 0)
		{
			table.erase(table.begin()+i);
			i--;
		}
	}
	for (short i = 0; i < table.size(); i++) {
		if (max_clk < table[i][2]) max_clk = table[i][2];
		cout << instrucitons[table[i][3]] << ": ";
		for (short j = 0; j < table[i].size()-1; j++) {
			cout << table[i][j] << " ";
		}
		cout << endl;
	}

	cout << "clock cycles required: " << max_clk << endl;
	cout << "CPI: " << double(max_clk) / table.size()<<endl;
	double branch_miss_p = double(branch_miss) / (branch_miss + branch_predict) ;
	cout << "Branch miss % = " << branch_miss_p*100<<endl;

	system("PAUSE");
	return 0;
}