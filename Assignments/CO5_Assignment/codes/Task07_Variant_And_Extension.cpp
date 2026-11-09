// Additional bill and payer extension example is fully implemented in Task01_Polymorphic_Hospital_Billing.cpp.
#include <iostream>
#include <variant>
#include <string>
struct OutPatientBill { double consultation, tests; double total() const{return consultation+tests;} };
struct InPatientBill { int days; double perDay, procedures; double total() const{return days*perDay+procedures;} };
using BillVariant=std::variant<OutPatientBill,InPatientBill>;
int main(){BillVariant bills[]={OutPatientBill{500,1200},InPatientBill{3,2500,40000}};
 double sum=0;for(const auto& b:bills)sum+=std::visit([](const auto& x){return x.total();},b);
 std::cout<<"variant total "<<sum<<'\n';}
