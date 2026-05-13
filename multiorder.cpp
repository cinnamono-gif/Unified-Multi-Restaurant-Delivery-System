#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Limits for fixed-size arrays
const int MAX=50;
// Limit for how many restaurants in one go (for input buffer)
const int MAX_REST_PER_ORDER=10; 

// Encapsulation & Basic Entities

enum OrderStatus{PENDING,ROUTED,PICKED_UP,DELIVERED,FAILED};

// Abstract Base Class
class IActor{
protected:
    int id;
    string name;
public:
    IActor():id(0),name(""){}
    IActor(int i,string n):id(i),name(n){}
    int getId()const{return id;}
    string getName()const{return name;}
};

// Inheritance
class Courier:public IActor{
    bool available;
public:
    Courier():IActor(),available(false){}
    Courier(int i,string n):IActor(i,n),available(true){}
    bool isAvailable()const{return available;}
    void setBusy(){available=false;}
};

// Encapsulation
class Order{
    int id;
    int restId;
    int custId;
    OrderStatus status;
public:
    Order():id(0),restId(0),custId(0),status(PENDING){}
    Order(int i,int r,int c):id(i),restId(r),custId(c),status(PENDING){}
    int getId()const{return id;}
    OrderStatus getStatus()const{return status;}
    void setStatus(OrderStatus s){status=s;}
};

// Secure Verification Module 

struct LogEntry{
    int orderId;
    string type;
    string actor;
};

class VerificationEngine{
    LogEntry logs[MAX];
    int logCount;
public:
    VerificationEngine():logCount(0){}
    void record(int oid,string type,string actor){
        if(logCount<MAX){
            logs[logCount].orderId=oid;
            logs[logCount].type=type;
            logs[logCount].actor=actor;
            logCount++;
            cout<<"[Log]Recorded: "<<type<<" for Order "<<oid<<" by "<<actor<<endl;
        }
    }
    bool verify(int oid){
        bool pickedUp=false;
        bool droppedOff=false;
        for(int i=0;i<logCount;i++){
            if(logs[i].orderId==oid){
                if(logs[i].type=="PICKUP")pickedUp=true;
                if(logs[i].type=="DROPOFF_CONFIRMED")droppedOff=true;
            }
        }
        return pickedUp&&droppedOff;
    }
};

//Routing Engine polymorphism

class IRouter{
public:
    virtual void route(Order* orders,int oCount,Courier* couriers,int cCount)=0;
};

class SimpleRouter:public IRouter{
public:
    void route(Order* orders,int oCount,Courier* couriers,int cCount)override{
        cout<<"\n[Router]Running multi-stop routing..."<<endl;
        Courier* selected=NULL;
        for(int i=0;i<cCount;i++){
            if(couriers[i].isAvailable()){
                selected=&couriers[i];
                break;
            }
        }
        if(selected!=NULL){
            cout<<"[Router]Assigning batch to Courier "<<selected->getName()<<endl;
            selected->setBusy();
            int routedCount=0;
            for(int i=0;i<oCount;i++){
                if(orders[i].getStatus()==PENDING){
                    orders[i].setStatus(ROUTED);
                    cout<<" -> Order "<<orders[i].getId()<<" added to route."<<endl;
                    routedCount++;
                }
            }
            if(routedCount==0)cout<<"[Router]No pending orders to route."<<endl;
        }else{
            cout<<"[Router]No available couriers found."<<endl;
        }
    }
};

// Main Backend Controller 

class Backend{
    Order orders[MAX];
    int oCount;
    Courier couriers[MAX];
    int cCount;
    VerificationEngine verifier;
    IRouter* router;
public:
    Backend(IRouter* r):oCount(0),cCount(0),router(r){}
    void addCourier(int id,string n){
        if(cCount<MAX){
            couriers[cCount++]=Courier(id,n);
            cout<<"[System]Courier added: "<<n<<endl;
        }else{
            cout<<"[System]Error: Courier limit reached."<<endl;
        }
    }
    void placeMultiOrder(int custId,int* rIds,int count){
        static int oidGen=1;
        if(oCount+count<MAX){
            cout<<"\n[System]Customer "<<custId<<" ordering from "<<count<<" restaurants."<<endl;
            // Loop through the input array to create orders
            for(int i=0;i<count;i++){
                 int oId=oidGen++;
                 orders[oCount++]=Order(oId,rIds[i],custId);
                 cout<<" -> Created Order ID: "<<oId<<" for Restaurant "<<rIds[i]<<endl;
            }        }else{
            cout<<"[System]Error: Order limit reached."<<endl;
        }
    }
    void runRouting(){
        router->route(orders,oCount,couriers,cCount);
    }
    void updateStatus(int oid,string type,string actor){
        bool found=false;
        for(int i=0;i<oCount;i++){if(orders[i].getId()==oid)found=true;}
        if(found){
            verifier.record(oid,type,actor);
            if(type=="DROPOFF_CONFIRMED"){
                completeOrder(oid);
            }
        }else{
            cout<<"[System]Error: Order ID "<<oid<<" not found."<<endl;
        }
    }
    void completeOrder(int oid){
        cout<<"\n[System]Attempting to verify and complete Order "<<oid<<"..."<<endl;
        if(verifier.verify(oid)){
            cout<<"[SUCCESS]Verification PASSED. Payment released."<<endl;
            for(int i=0;i<oCount;i++){if(orders[i].getId()==oid)orders[i].setStatus(DELIVERED);}
        }else{
            cout<<"[ALERT]Verification FAILED. Missing confirmation steps."<<endl;
        }
    }
};

void clearInput(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

int main(){
    SimpleRouter rLogic;
    Backend sys(&rLogic);

    sys.addCourier(501,"Imad");

    int choice=0;
    while(choice!=6){
        cout<<"\n=== Delivery Backend Menu ==="<<endl;
        cout<<"1. Add New Courier"<<endl;
        cout<<"2. Place Multi-Restaurant Order (Any Number)"<<endl;
        cout<<"3. Run Routing Engine (Assign Pending Orders)"<<endl;
        cout<<"4. Update Order Status (Pickup/Dropoff)"<<endl;
        cout<<"5. Force Verify/Complete an Order"<<endl;
        cout<<"6. Exit"<<endl;
        cout<<"Enter choice: ";
        if(!(cin>>choice)){
            cout<<"Invalid input. Please enter a number."<<endl;
            clearInput();
            continue;
        }
        clearInput();

        switch(choice){
            case 1:{
                int id;string name;
                cout<<"Enter Courier ID (number): ";cin>>id;
                clearInput();
                cout<<"Enter Courier Name: ";getline(cin,name);
                sys.addCourier(id,name);
                break;
            }
            case 2:{
                int cId,count;
                int tempRIds[MAX_REST_PER_ORDER]; // Temp buffer for inputs
                cout<<"Enter Customer ID: ";cin>>cId;
                cout<<"How many restaurants are you ordering from? (Max "<<MAX_REST_PER_ORDER<<"): ";
                cin>>count;
                
                if(count>0&&count<=MAX_REST_PER_ORDER){
                     for(int i=0;i<count;i++){
                         cout<<"Enter Restaurant ID #"<<(i+1)<<": ";
                         cin>>tempRIds[i];
                     }
                     clearInput();
                     // Pass the array and count to backend
                     sys.placeMultiOrder(cId,tempRIds,count);
                }else{
                     cout<<"Invalid number of restaurants."<<endl;
                     clearInput();
                }
                break;
            }
            case 3:{
                sys.runRouting();
                break;
            }
            case 4:{
                int oid;int typeChoice;string actor;
                cout<<"Enter Order ID to update: ";cin>>oid;
                cout<<"Select Status Type:\n 1. PICKUP (by courier)\n 2. DROPOFF_CONFIRMED (by customer)\nChoice: ";
                cin>>typeChoice;
                clearInput();
                cout<<"Enter Actor Name (who is doing this action): ";getline(cin,actor);

                string typeStr="";
                if(typeChoice==1)typeStr="PICKUP";
                else if(typeChoice==2)typeStr="DROPOFF_CONFIRMED";

                if(typeStr!="") sys.updateStatus(oid,typeStr,actor);
                else cout<<"Invalid status type selected."<<endl;
                break;
            }
            case 5:{
                 int oid;
                 cout<<"Enter Order ID to try complete: ";cin>>oid;
                 clearInput();
                 sys.completeOrder(oid);
                 break;
            }
            case 6:{
                cout<<"Exiting..."<<endl;
                break;
            }
            default:{
                cout<<"Invalid choice."<<endl;
            }
        }
    }
    return 0;
}
