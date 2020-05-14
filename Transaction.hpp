#ifndef _TRANSACTION_HPP_
#define _TRANSACTION_HPP_

#include <iomanip>
#include <iostream>
#include <string>

/////////////////////////////////////////////////////////////////////////
//ECE 150 - PROJECT 3
//
//File:					Transaction.hpp
//Authors: 				Stephanie De Pol (Functions labeled "TASK")
//                      Douglas Harder (Functions labeled "GIVEN")
//                      Hiren Patel (Sections labeled "GIVEN")
//Program Objective: 	Support program for History_Transaction_definitions.hpp
////////////////////////////////////////////////////////////////////////

class Transaction {
  public:
    // TASK: Constructor
	Transaction( std::string ticker_symbol,  unsigned int day_date,
        unsigned int month_date,  unsigned year_date,
        bool buy_sell_trans,  unsigned int number_shares,
        double trans_amount );

    // TASK: Destructor
    ~Transaction();


    // TASK: Overloaded < operator.
    bool operator < ( Transaction const &other );

    // GIVEN: Member functions to get values.
    std::string get_symbol() const;
    unsigned int get_day() const;
    unsigned int get_month() const;
    unsigned int get_year() const;
    unsigned int get_shares() const;
    double get_amount() const;
    double get_acb() const;
    double get_acb_per_share() const;
    unsigned int get_share_balance() const;
    double get_cgl() const;
    bool get_trans_type() const;
    unsigned int get_trans_id() const;
    Transaction *get_next();

    // GIVEN: Member functions to set values.
    void set_acb( double acb_value );
    void set_acb_per_share( double acb_share_value );
    void set_share_balance( unsigned int bal );
    void set_cgl( double value );
    void set_next( Transaction *p_new_next );

    // GIVEN: Print the transaction.
    void print();
    private:
    std::string symbol;
    unsigned int day;
    unsigned int month;
    unsigned int year;
    std::string trans_type;
    unsigned int shares;
    double amount;
    unsigned int trans_id;

    // These private members have to be populated.
    double acb;
    double acb_per_share;
    unsigned int share_balance;
    double cgl;

    Transaction *p_next;

    // GIVEN: A unique identifier class variable to assign identifiers (trans_id).
    static unsigned int assigned_trans_id;

};
#endif