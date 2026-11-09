#include <iostream>
#include <string>
using namespace std;

class SavingsAccount {
private:
    string accountNumber;
    long long balancePaise;
    long long transactions[10];
    int count;
    int nextIndex; // next position to overwrite in circular buffer
public:
    SavingsAccount(const string& number, long long openingPaise)
        : accountNumber(number), balancePaise(openingPaise), transactions{}, count(0), nextIndex(0) {}
    long long getBalance() const { return balancePaise; } // inline inside class
    void deposit(long long amountPaise);
    bool withdraw(long long amountPaise);
    void miniStatement() const;
};
void SavingsAccount::deposit(long long amountPaise) {
    if (amountPaise<=0) return;
    balancePaise+=amountPaise;
    transactions[nextIndex]=amountPaise;
    nextIndex=(nextIndex+1)%10;
    if (count<10) ++count;
}
bool SavingsAccount::withdraw(long long amountPaise) {
    if (amountPaise<=0 || amountPaise>balancePaise) return false;
    balancePaise-=amountPaise;
    transactions[nextIndex]=-amountPaise;
    nextIndex=(nextIndex+1)%10;
    if (count<10) ++count;
    return true;
}
void SavingsAccount::miniStatement() const {
    cout << "Account: " << accountNumber << "\nBalance (paise): " << balancePaise << '\n';
    cout << "Last " << count << " transactions (oldest to newest):\n";
    int start=(count==10)?nextIndex:0;
    for (int i=0;i<count;++i) {
        int pos=(start+i)%10;
        cout << (transactions[pos]>=0?"+":"") << transactions[pos] << '\n';
    }
}
int main() {
    SavingsAccount account("SA1001",100000);
    account.deposit(5000);
    account.withdraw(2000);
    for (int i=1;i<=10;++i) account.deposit(i*100);
    account.miniStatement();
    cout << "Final balance (paise): " << account.getBalance() << '\n';
}
