#include <iostream>

using namespace std;

class Queue
{
    public:
        int items[10], front, rear;
        static const int size = 10;

    public:
        Queue(){
            front = -1;
            rear = -1;
        }

        bool isFull(){
            return rear == size -1;
        }

        bool isEmpty(){
            return front == -1;
        }

        void enqueue(int x) {
        if (isFull())
            std::cout << "Queue is Full" << std::endl;
        else {
            if (front == -1)
                front = 0;
            rear++;
            items[rear] = x;
            std::cout << "Inserted " << x << std::endl;
        }
    }

        int dequeue() {
        int x;
        if (isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            return -1;
        } else {
            x = items[front];
            if (front >= rear) {
                front = -1;
                rear = -1;
            } else
                front++;
        }
        std::cout << "Deleted " << x << std::endl;
        return x;
    }
};

int main()
{
    Queue myQueue; // Create an instance of the Queue class
    cout << "Hello world!" << endl;
    int x = myQueue.dequeue();
    cout << "x = "<< x << endl;
    myQueue.enqueue(2);
    cout << "front = " << myQueue.front << endl;
    cout << "rear = " << myQueue.rear << endl;
    for (int i = 0; i < 7;i++){
        myQueue.enqueue(i+1);
    }
    cout << "front " << myQueue.front << endl;
    cout << "rear = " << myQueue.rear << endl;
    bool z = myQueue.isFull();
    cout << "isFull is " << z << endl;
    int y = myQueue.dequeue();
    cout << "y = "<< y << endl;
    for (int i = 0; i < 7;i++){
        myQueue.dequeue();
    }
    bool s = myQueue.isEmpty();
    cout << "isEmpty is " << s << endl;
    return 0;

}

