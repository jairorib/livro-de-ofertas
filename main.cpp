#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

int generate_random(int min, int max) {

  std::random_device rd; // Obtém uma semente aleatória
  std::mt19937 gen(rd()); // Inicializa o gerador com a semente
  std::uniform_int_distribution<> dis(min, max);

  return dis(gen);
}

double generate_value(double min, double max) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min, max);

    return dis(gen);
}

double toFixed(double number) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << number;
    return std::stod(out.str());
}

class Order {
public:
    int index;
    double price;
    int quantity;
    char type; // 'b' for buy, 's' for sell

    Order(int i,double p, int q, char t) : index(i), price(p), quantity(q), type(t) {}
};

bool modificarPreco(vector<Order>& orders, const int& index) {
    double novoPreco = 0.10;

    for (auto& order : orders) {
        if (order.index == index && order.quantity != 0) { // Verifica se o nome do produto corresponde
            order.price += novoPreco; // Substitui o preço
            return true;
            break; // Sai do loop após a substituição
        }
    }
    return false;
};

double checkValue(std::vector<Order> orders) {
         if (!orders.empty()) {
            return orders.back().price;
         }
         return 1.01;
        }

class Notification {
public:
    int index;
    int type;
    double price;
    int quantity;

    Notification(int i, int t, double p, int q) : index(i), type(t), price(p), quantity(q) {}
};

class OrderBook {
private:
    vector<Order> buy_orders;
    vector<Order> sell_orders;
    vector<Notification> message;

public:

    void add_order(int min, int max) {

        char chars[] = {'b', 's'};
        double priceB = toFixed(generate_value(checkValue(buy_orders), generate_value(1.01, 1.5) * max));
        double priceS = toFixed(generate_value(checkValue(sell_orders), generate_value(1.01, 1.5) * max));
        int quantity = generate_random(1, 200);
        char type = chars[generate_random(0, 1)];

        if (type == 'b') {
            buy_orders.emplace_back(quantity, priceB, quantity, type);
        } else {
            int posicao = (!sell_orders.empty()) ? sell_orders.size()+1 : 1;
            sell_orders.emplace_back(posicao, priceS, quantity, type);
            message.emplace_back(posicao, 0, priceS, quantity);
        }
        match_orders();
    }

    void get_message() {
        cout << "(" << message.size() << ") Notificações :::" << endl;
        for (auto& msg : message) {
          cout << "(" << msg.index << "," << msg.type << "," << msg.price << "," << msg.quantity << ")" << endl;
        }
        cout << "---------------" << endl;
        get_result();
    }

    void get_result() {
        cout << "Resultado :::" << endl;
        for (auto& msg : sell_orders) {
           cout << "(" << msg.index << ","<< msg.price << "," << msg.quantity << ")" << endl;
        }
    }

    void match_orders() {
        while (!buy_orders.empty() && !sell_orders.empty() && buy_orders.front().price >= sell_orders.front().price) {
            Order& buy_order = buy_orders.front();
            Order& sell_order = sell_orders.front();

            int min_quantity = (buy_order.quantity < sell_order.quantity) ? buy_order.quantity : sell_order.quantity;

            buy_order.quantity -= min_quantity;
            sell_order.quantity -= min_quantity;

            if (buy_order.quantity == 0) buy_orders.erase(buy_orders.begin());
            if (sell_order.quantity == 0) {

              sell_orders.erase(sell_orders.begin());
              message.emplace_back(sell_order.index, 2, 0, 0);

            } else {
              message.emplace_back(sell_order.index, 1, 0, sell_order.quantity);
              if (generate_random(0,1) == 0) {

                auto it = sell_orders.begin();
                auto nextIt = next(it);

                modificarPreco(sell_orders, sell_order.index + 1);
                message.emplace_back(sell_order.index + 1, 1, nextIt->price,0);
              };
            }
        }
    }
};

int main() {
    OrderBook book;

    int operation = 0;

    int limit = generate_random(10, 20);
    int fragment = limit;

    while (operation <= limit) {

      book.add_order(operation, 21 - fragment);
      fragment--;
      operation++;
    }
    book.get_message();

    return 0;
}
