#ifndef TKOM_HEXGRID_ERRORS_H
#define TKOM_HEXGRID_ERRORS_H
#include <string>
#include <sstream>
#include <exception>
#include <iostream>
#include <utility>
#include <string>
namespace hexgrid_errors
{
    class HexgriderException : public std::exception{
        public:
            const char* what() const noexcept {
                return msg.c_str();
            }       
        protected:
            std::string pos_text(std::pair<int, int> loc){
                return "(" + std::to_string(loc.first) + ", " + std::to_string(loc.second) + ")";
            };

            std::string msg;
    };
    
    class UnkownCharacterException : public HexgriderException
    {
        public:
        UnkownCharacterException(std::pair<int, int> loc, char character){
            std::stringstream ssmsg;
            ssmsg << pos_text(loc) << " Unkown character " << "\"" << character << "\"";
            msg = ssmsg.str();   
        };
    };
    
    class BadEndOfInput : public HexgriderException
    {
        public:
        BadEndOfInput(){
            msg = "The end of input was not EOF";
        }
    };
    
    class OpenQuotes : public HexgriderException
    {
        public:
        OpenQuotes(std::pair<int, int> start){
            msg = pos_text(start) + " Quotes opened, but not closed";
        }
    };

    class IntegerOverflow : public HexgriderException{
        public:
        IntegerOverflow(std::pair<int, int> start){
            msg = pos_text(start) + " Integer can only contain value from -2,147,483,647 to 2,147,483,647";
        }
    };

    class UnkownEscapeCombination : public HexgriderException{
        public:
        UnkownEscapeCombination(std::pair<int, int> start, char symb){
            std::stringstream ssmsg;
            ssmsg << pos_text(start) << " Unkown escape combination \"\\" << symb << "\"";
            msg = ssmsg.str();
        }
    };

    class UnkownOperator : public HexgriderException{
        public:
        UnkownOperator(std::pair<int, int> start, std::string symb){
            std::stringstream ssmsg;
            ssmsg << pos_text(start) << " Unkown operator \"\\" << symb << "\"";
            msg = ssmsg.str();
        }
    };

    class WrongTokenConstructor : public HexgriderException{
        public:
        WrongTokenConstructor(std::pair<int, int> start, std::string expected, std::string given){
            std::stringstream ssmsg;
            ssmsg << pos_text(start) << " Cannot construct " << expected << " token with " << given << " value";
            msg = ssmsg.str();
        }
    };

    class UnexpectedInput : public HexgriderException{
        public:
        UnexpectedInput(std::pair<int, int> start, std::string expected, std::string given){
            std::stringstream ssmsg;
            ssmsg << pos_text(start) << " Recieved unexpected input: " << expected << ", expected " << given << "\n";
            msg = ssmsg.str();
        }
    };

    class FunctionRedefinition : public HexgriderException{
        public:
        FunctionRedefinition(std::pair<int, int> start, std::pair<int, int> end, std::string func_name){
            std::stringstream ssmsg;
            ssmsg << "From "<< pos_text(start) << " to " << pos_text(end) << " Redefinition of a function: " << func_name << "\n";
            msg = ssmsg.str();
        }
    };

    class VariableIsNotDeclared : public HexgriderException{
        public:
        VariableIsNotDeclared(/* std::pair<int, int> start,  */std::string var_name){
            std::stringstream ssmsg;
            ssmsg << /* pos_text(start) <<  */" Reference to undefined variable: " << var_name << "\n";
            msg = ssmsg.str();
        }
    };

    class AssingingWrongVariableType : public HexgriderException{
        public:
        AssingingWrongVariableType(std::string expected_type, std::string given_type){
            std::stringstream ssmsg;
            ssmsg << " recieved " << given_type << ", expected " << expected_type << "\n";
            msg = ssmsg.str();
        }
    };

    class GettingWrongVariableType : public HexgriderException{
        public:
        GettingWrongVariableType(std::string expected_type, std::string given_type){
            std::stringstream ssmsg;
            ssmsg << " recieved " << given_type << ", expected " << expected_type << "\n";
            msg = ssmsg.str();
        }
    };

    class OperationsNotAvailabledForTypes : public HexgriderException{
        public:
        OperationsNotAvailabledForTypes(std::string op, std::string type1, std::string type2){
            std::stringstream ssmsg;
            ssmsg << " Cannot perform operation " << op << " on types " << type1 << " and " << type2 << '\n';
        }
    };

} //namespace hexgrid_erorrs

#endif //TKOM_HEXGRID_ERRORS_H