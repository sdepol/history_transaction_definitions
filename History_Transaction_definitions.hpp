#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

/////////////////////////////////////////////////////////////////////////
//ECE 150 - PROJECT 3
//
//File:					History_Transaction_definitions.hpp
//Authors: 				Stephanie De Pol (Functions labeled "TASK")
//                      Douglas Harder (Functions labeled "GIVEN")
//                      Hiren Patel (Sections labeled "GIVEN")
//Program Objective: 	Use linked lists in order to store, sort, and make
//                      calculations based on inputted transaction history.
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////

// TASK: Constructor
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date, unsigned int month_date,  unsigned year_date,
        					bool buy_sell_trans,  unsigned int number_shares, double trans_amount ){

	    //Initialize all of the member variables of Transaction class.
	    //To do so, we set all known member variables to their respective parameter, and set all unknown member variables to zero.
		this->symbol = ticker_symbol;
		this->day = day_date;
		this->month = month_date;
		this->year = year_date;

		if(buy_sell_trans){
			this->trans_type = "Buy";
		}else{
			this->trans_type = "Sell";
		}

		this->shares = number_shares;

		this->amount = trans_amount;

		this -> share_balance = 0.0;

		this -> trans_id = Transaction::assigned_trans_id;
		Transaction::assigned_trans_id++;

		this->p_next = nullptr;

		this->acb=0;
		this->acb_per_share=0;
		this->share_balance=0;
		this->cgl=0;
    }


// TASK: Destructor
Transaction::~Transaction(){
	//No member variables in Transaction class are dynamically allocated so the destructor does not need to deallocated any memory.
}



// TASK: Overloaded < operator.
bool Transaction::operator < (Transaction const & other){
	    //Compare the years of the two Transaction variables.
		if(this->year < other.year){
			return true;
		} else if(this->year > other.year){
			return false;
		}
		assert (this->year == other.year);
		//At this point in the code, the years are the same.

		//Compare the months of the two Transaction variables.
		if(this->month < other.month){
			return true;
		} else if(this->month > other.month){
			return false;
		}
		assert (this->month == other.month);
		//At this point in the code, the months are the same.

		//Compare the days of the two Transaction variables.
		if(this->day < other.day){
			return true;
		} else if(this->day > other.day){
			return false;
		}
		assert (this->day == other.day);
		//At this point in the code, the days are the same.

		//Since in this case it would be the same day, technically it is not less than. Thus, we return false.
		return false;
}

// GIVEN: Member functions to get values.
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN: Member functions to set values.
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN: Print the transaction.
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////


// TASK: Constructor
History::History(){
	//Initialize the member variable p_head to nullptr to create an empty linked list.
	p_head=nullptr;
}

// TASK: Destructor
History::~History(){
	Transaction * p_transverse {p_head};

	//Since the linked list is dynamically allocated, we must walk through the linked list deallocating each node one at a time.
	//After we deallocate a node, we change the head of the linked list to the next node and repeat the process.
	while(p_transverse != nullptr){
		p_transverse = p_transverse->get_next();
		delete p_head;
		p_head=p_transverse;
	}
}

// TASK: read_history(...) Read the transaction history from file.
void History::read_history(){
	ece150::open_file();
	//Our task is to create a linked list with all of the transactions from the transaction_history text file
	//To do so, we first use a while loop with ece150::next_trans_entry() as it's condition to loop through the text file

	while(ece150::next_trans_entry()){

		//We then create a variable of type transaction initialized with all of the values from a line in the text file
		Transaction *p_node = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(),
			ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(),
			ece150::get_trans_shares(), ece150::get_trans_amount());

		//Using our insert member function, we insert this new Transaction variable into the linked list
		History::insert(p_node);
	}

	ece150::close_file();
}

// TASK: insert(...) Insert transaction into linked list.
void History::insert(Transaction *p_new_trans){

	//First check if linked list is empty.
	if(p_head==nullptr){
		p_head = p_new_trans;
	}else{

	//If it is not empty, we must append the node from the back.
	Transaction *p_transverse = p_head;

	//First, walk through the linked list until we get to the last node (this is the node that points to the nullptr).
	while(p_transverse-> get_next()!=nullptr){
		p_transverse=p_transverse -> get_next();
	}

	//For the last node, we set the next node after it to be the new node.
	//Then, for the new node, we set its next node to be nullptr.
	p_transverse->set_next(p_new_trans);
	p_new_trans -> set_next(nullptr);
	}
}


// TASK: sort_by_date() Sort the linked list by trade date.
void History::sort_by_date(){
	//p_temp will represent the node removed from the unsorted linked list that will be placed in the sorted linked list.
	Transaction *p_temp{nullptr};
	//p_old will represent the head of the original unsorted linked list, excluding the nodes that have already been placed in the sorted linked list.
	Transaction *p_old{p_head};
	//p_new will represent the head of the sorted linked list.
	Transaction *p_new{nullptr};

	//To sort, we must loop through each node in the original unsorted linked list and sort each node individually (insertion sort).
	while(p_old != nullptr){

		//We first isolate the head of the unsorted linked list by storing it in p_temp.
		//We then shift the head of the unsorted linked list to the next node by using the get next function.
		p_temp = p_old;
		p_old = p_old->get_next();

		//There are four ways in which p_temp can be inserted into the sorted linked list.
		if (p_new == nullptr){
			//Case 1: The sorted list is empty so p_temp becomes the head of the sorted linked list, and the node after p_temp is set to the nullptr.
			p_new = p_temp;
			p_new->set_next(nullptr);
		}else if (*p_temp < *p_new){
			//Case 2: p_temp is less than the head of the sorted linked list, therefore it must be appended to the front.
			p_temp->set_next(p_new);
			p_new = p_temp;
		}else if (p_new->get_next() == nullptr){
			//Case 3: p_temp is greater than the head of the sorted linked list, therefore it must be appended to the back.
			p_new->set_next(p_temp);
			p_temp->set_next(nullptr);
		}else{
			//Case 4A: p_temp is the greatest in the sorted linked list and should be placed at the end of the list.
			//Case 4B: p_temp must be placed in between two nodes in the sorted linked list.

			//Initialize p_traverse to p_new in order to preserve the head of the sorted linked list
			Transaction *p_traverse{p_new};

			//Walk through the liked list until you reach a node greater than p_temp or the last node of the sorted linked list.
			while((p_traverse->get_next() != nullptr) && (*(p_traverse->get_next()) < *p_temp)){
				p_traverse = p_traverse->get_next();
			}
			//Append p_temp so it is placed after the node at p_transverse.
			p_temp->set_next(p_traverse->get_next());
			p_traverse->set_next(p_temp);
		}


	}
	//Once all of the sorting is done in the sorted linked list, set p_head so it is now the head of the sorted linked list (p_new).
	p_head= p_new;
}


// TASK: update_acb_cgl() Updates the ACB and CGL values.
void History::update_acb_cgl(){

Transaction *temp_node{p_head};
double acb{};
unsigned int share_b{};
double acb_share{};
double cgl{};

//Using the do-while loop, we walk through the linked list and complete computations on each individual transaction.
do{
	if(temp_node->get_trans_type()){
	//In this case, the transaction type is a buy.

	//To get the ACB, simply add the amount paid to the total ACB.
	acb += temp_node->get_amount();
	temp_node->set_acb(acb);

	//To get the Share Balance, simply add the Share Balance to the previous Share Balance.
	share_b += temp_node->get_shares();
	temp_node->set_share_balance(share_b);

	//To get the ACB per Share, divide the ACB by the Share Balance.
	acb_share = temp_node->get_acb()/temp_node->get_share_balance();
	temp_node->set_acb_per_share(acb_share);

	}else{
	//In this case, the transaction type is a sell.

	//To get the ACB, subtract the number of shares sold multiplied by the previous ACB/Share.
	acb -=(temp_node->get_shares() * acb_share);
	temp_node->set_acb(acb);

	//To get the Share Balance, simply subtract the Share Balance from the previous Share Balance.
	share_b -= temp_node->get_shares();
	temp_node->set_share_balance(share_b);

	//To get the ACB per Share, divide the ACB by the Share Balance.
	acb_share = (temp_node->get_acb())/(temp_node->get_share_balance());
	temp_node->set_acb_per_share(acb_share);

	//To get the CGL, subtract the number of shares sold multiplied by the previous ACB/Share.
	cgl = temp_node->get_amount() - (temp_node->get_shares()*temp_node->get_acb_per_share());
	temp_node->set_cgl(cgl);
	}

	temp_node = temp_node->get_next();
}while(temp_node!=nullptr);

}


// TASK: compute_cgl() Compute the ACB, and CGL.
double History::compute_cgl(unsigned int year){
	double cgl_per_year{0};
	Transaction *temp_node{p_head};

	//To calculate the total CGL, walk through the linked list adding the CGL for each transaction of the specified year.
	do{
		if(temp_node->get_trans_type() == false){
			if(temp_node->get_year() == year){
				cgl_per_year += temp_node->get_cgl();
			}
		}
		temp_node = temp_node->get_next();
	}while(temp_node!=nullptr);

	return cgl_per_year;
}



// TASK: print() Print the transaction history.
void History::print(){
	std::cout << "===== BEGIN TRANSATION HISTORY =====" << std::endl;

	//To print, begin at the head and walk through the linked list, printing each node until you reach the nullptr.
	Transaction *p_traverse {p_head};
	do{
		p_traverse->print();
		p_traverse = p_traverse->get_next();
	}while(p_traverse != nullptr);

	std::cout << "===== END TRANSATION HISTORY =====" << std::endl;
}



// GIVEN: get_p_head(): Full access to the linked list.
Transaction *History::get_p_head() { return p_head; }


#endif

