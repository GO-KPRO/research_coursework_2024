#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <functional>

class Place
{
public:
    std::string name;
    bool token;
    int number;

    Place(bool token, std::string name, int number)
    {
        this->token = token;
        this->name = name;
        this->number = number;
    }

    bool operator ==(Place place)
    {
        if ((place.name == this->name) && (place.token == this->token) && (place.number == this->number)) {
            return true;
        }
        return false;
    }

    bool operator !=(Place place) {
        return !(*this == place);
    }
};

class Transition
{
public:
    std::string name;
    std::vector<int> inputPlacesNumbers;
    std::vector<int> outputPlacesNumbers;

    Transition(std::string name, std::vector<int> inputPlacesNumbers, std::vector<int> outputPlacesNumbers)
    {
        this->name = name;
        this->inputPlacesNumbers = inputPlacesNumbers;
        this->outputPlacesNumbers = outputPlacesNumbers;
    }

    bool operator ==(Transition transition) {
        if (transition.name == this->name) {
            if (transition.inputPlacesNumbers.size() == this->inputPlacesNumbers.size()) {
                if (transition.outputPlacesNumbers.size() == this->outputPlacesNumbers.size()) {
                    for (size_t i = 0; i < this->inputPlacesNumbers.size(); ++i) {
                        if (this->inputPlacesNumbers[i] != transition.inputPlacesNumbers[i]) {
                            return false;
                        }
                    }
                    for (size_t i = 0; i < this->outputPlacesNumbers.size(); ++i) {
                        if (this->outputPlacesNumbers[i] != transition.outputPlacesNumbers[i]) {
                            return false;
                        }
                    }
                    return true;
                }
            }
        }
        return false;
    }

    bool operator !=(Transition transition) {
        return !(*this == transition);
    }
};

class Step
{
public:
    int transitionPosition;
    std::string transitionName;
    std::string netName;

    Step() {};

    Step(int transitionPosition, std::string transitionName, std::string netName) {
        this->transitionPosition = transitionPosition;
        this->transitionName = transitionName;
        this->netName = netName;
    }

    void out() {
        if (transitionPosition >= 0) {
            std::cout << netName << " " << transitionName << " " << transitionPosition << std::endl;
        }
        else {
            std::cout << "No way!" << std::endl;
        }
    }
};

class Net
{
public:
    std::string name;
    std::vector<Place> places;
    std::vector<Transition> transitions;

    Net() {};

    Net(std::string name) {
        this->name = name;
    }

    bool operator ==(Net net) {
        if (this->name == net.name) {
            if (this->places.size() == net.places.size()) {
                if (this->transitions.size() == net.transitions.size()) {
                    for (size_t i = 0; i < this->places.size(); ++i) {
                        if (this->places[i] != net.places[i]) {
                            return false;
                        }
                    }
                    for (size_t i = 0; i < this->transitions.size(); ++i) {
                        if (this->transitions[i] != net.transitions[i]) {
                            return false;
                        }
                    }
                    return true;
                }
            }
        }
        return false;
    }

    bool operator !=(Net net) {
        return !(*this == net);
    }

    void setPlaces(std::vector<std::string> placesNames, std::vector<bool> markup) {
        for (size_t i = 0; i < placesNames.size(); ++i) {
            places.push_back(Place(markup[i], placesNames[i], i));
        }
    }

    void setTransitions(std::vector<std::string> transitionsNames, std::vector<std::vector<int>> inputPlacesNumbers, std::vector<std::vector<int>> outputPlacesNumbers) {
        for (size_t i = 0; i < transitionsNames.size(); ++i) {
            transitions.push_back(Transition(transitionsNames[i], inputPlacesNumbers[i], outputPlacesNumbers[i]));
        }
    }

    bool checkTransition(size_t num) {
        for (size_t i = 0; i < transitions[num].inputPlacesNumbers.size(); ++i) {
            if (!places[transitions[num].inputPlacesNumbers[i]].token)
                return false;
        }
        return true;
    }

    bool makeTransition(size_t num) {
        bool res = checkTransition(num);
        if (res) {
            for (size_t i = 0; i < transitions[num].inputPlacesNumbers.size(); ++i) {
                places[transitions[num].inputPlacesNumbers[i]].token = false;
            }
            for (size_t i = 0; i < transitions[num].outputPlacesNumbers.size(); ++i) {
                places[transitions[num].outputPlacesNumbers[i]].token = true;
            }
            return true;
        }
        return false;
    }

    int findRandomAvailableTransition() {
        std::vector<size_t> availableTransitions;
        for (size_t i = 0; i < transitions.size(); ++i) {
            if (checkTransition(i))
                availableTransitions.push_back(i);
        }

        if (availableTransitions.size() > 0) {
            std::random_device rd;
            std::uniform_int_distribution<int> dist(0, availableTransitions.size() - 1);
            return availableTransitions[dist(rd)];
        }
        else {
            return -1;
        }
    }

    int findRandomAvailableTransition(std::string name) {
        std::vector<size_t> availableTransitions;
        for (size_t i = 0; i < transitions.size(); ++i) {
            if (checkTransition(i) && (transitions[i].name == name))
                availableTransitions.push_back(i);
        }

        if (availableTransitions.size() > 0) {
            std::random_device rd;
            std::uniform_int_distribution<int> dist(0, availableTransitions.size() - 1);
            return availableTransitions[dist(rd)];
        }
        else {
            return -1;
        }
    }

    void printAllAvailableTransitions() {
        for (size_t i = 0; i < transitions.size(); ++i) {
            if (checkTransition(i))
                std::cout << name << " " << transitions[i].name << " " << i << std::endl;
        }
    }

    void printAllAvailableTransitions(std::string transitionName) {
        for (size_t i = 0; i < transitions.size(); ++i) {
            if (checkTransition(i) && (transitionName == transitions[i].name))
                std::cout << name << " " << transitions[i].name << " " << i << std::endl;
        }
    }

    int countAvailableTransitions(std::string transitionName) {
        int res = 0;
        for (size_t i = 0; i < transitions.size(); ++i) {
            if (checkTransition(i) && (transitionName == transitions[i].name))
                ++res;
        }
        return res;
    }

    std::vector<Step> allAvailableSteps() {
        std::vector<Step> res;
        for (size_t i = 0; i < transitions.size(); ++i) {
            if (checkTransition(i))
                res.push_back(Step(i, transitions[i].name, name));
        }
        return res;
    }

    std::vector<Step> allAvailableSteps(std::string transitionName) {
        std::vector<Step> res;
        for (size_t i = 0; i < transitions.size(); ++i) {
            if (checkTransition(i) && (transitionName == transitions[i].name))
                res.push_back(Step(i, transitions[i].name, name));
        }
        return res;
    }

    std::string createSubgraph() {
        std::string res = "subgraph cluster_" + name + "{\n";
        res += "label=" + name + "\n";
        for (size_t i = 0; i < places.size(); ++i) {
            res += places[i].name + " [label=\"" + places[i].name + "\\n";
            if (places[i].token) {
                res += "*";
            }
            res += "\", shape=circle];\n";
        }

        for (size_t i = 0; i < transitions.size(); ++i) {
            std::string curTransName = name + "_" + transitions[i].name + "_" + std::to_string(i);
            res += curTransName + "[label=" + transitions[i].name + ", style = filled, fillcolor = gray, shape = rectangle];\n";

            for (size_t j = 0; j < transitions[i].inputPlacesNumbers.size(); ++j) {
                res += places[transitions[i].inputPlacesNumbers[j]].name + " -> " + curTransName + ";\n";
            }
            for (size_t j = 0; j < transitions[i].outputPlacesNumbers.size(); ++j) {
                res += curTransName + " -> " + places[transitions[i].outputPlacesNumbers[j]].name + ";\n";
            }
        }

        res += "}\n";

        return res;
    }
};

class Bisimulation
{
public:
    std::string name;
    Net first;
    Net second;

    Bisimulation() {};

    Bisimulation(std::string name, Net first, Net second) {
        this->name = name;
        this->first = first;
        this->second = second;
    }

    bool operator ==(Bisimulation bisimulation) {
        if ((this->name == bisimulation.name) && (this->first == bisimulation.first) && (this->second == bisimulation.second)) {
            return true;
        }
        return false;
    }

    bool operator !=(Bisimulation bisimulation) {
        return !(*this == bisimulation);
    }

    bool makeStep(Step step) {
        if (step.transitionPosition < 0) {
            return false;
        }
        if (step.netName == first.name) {
            return first.makeTransition(step.transitionPosition);
        }
        else {
            return second.makeTransition(step.transitionPosition);
        }
    }

    std::string plotGraph(std::string graphName) {
        std::string res = "digraph " + graphName + " {\n";

        res += first.createSubgraph();
        res += second.createSubgraph();

        res += "}";

        std::ofstream fout("dot\\" + graphName + ".dot");
        fout << res;
        fout.close();

        std::string command = "dot -Tpng dot\\" + graphName + ".dot > graphs\\" + graphName + ".png";

        system(command.c_str());

        return graphName;
    }

    std::string plotGraph(std::string graphName, Step attack) {
        std::string res = "digraph " + graphName + " {\n";

        res += first.createSubgraph();
        res += second.createSubgraph();

        res += attack.netName + "_" + attack.transitionName + "_" + std::to_string(attack.transitionPosition) + "[fillcolor=red]\n";

        res += "}";

        std::ofstream fout("dot\\" + graphName + ".dot");
        fout << res;
        fout.close();

        std::string command = "dot -Tpng dot\\" + graphName + ".dot > graphs\\" + graphName + ".png";

        system(command.c_str());

        return graphName;
    }

    std::string plotGraph(std::string graphName, Step attack, Step defend) {
        std::string res = "digraph " + graphName + " {\n";

        res += first.createSubgraph();
        res += second.createSubgraph();

        res += attack.netName + "_" + attack.transitionName + "_" + std::to_string(attack.transitionPosition) + "[fillcolor=red]\n";
        res += defend.netName + "_" + defend.transitionName + "_" + std::to_string(defend.transitionPosition) + "[fillcolor=green]\n";

        res += "}";

        std::ofstream fout("dot\\" + graphName + ".dot");
        fout << res;
        fout.close();

        std::string command = "dot -Tpng dot\\" + graphName + ".dot > graphs\\" + graphName + ".png";

        system(command.c_str());

        return graphName;
    }

    void printAvalibleTransitions() {
        first.printAllAvailableTransitions();
        second.printAllAvailableTransitions();
    }
};

bool isInHistory(Bisimulation bisimulation, std::vector<Bisimulation> history) {
    for (size_t i = 0; i < history.size(); ++i) {
        if (bisimulation == history[i]) {
            return true;
        }
    }
    return false;
}

Step randomAttack(Bisimulation bisimulation, std::vector<Bisimulation> history) {
    Net first = bisimulation.first;
    Net second = bisimulation.second;
    std::vector<Step> availableSteps = first.allAvailableSteps();
    std::vector<Step> secondAvailableSteps = second.allAvailableSteps();
    availableSteps.insert(availableSteps.end(), secondAvailableSteps.begin(), secondAvailableSteps.end());

    std::vector<Step> cur;

    for (size_t i = 0; i < availableSteps.size(); ++i) {
        Bisimulation curBis = Bisimulation(bisimulation);
        curBis.makeStep(availableSteps[i]);
        if (!isInHistory(curBis, history)) {
            cur.push_back(availableSteps[i]);
        }
    }

    if (cur.size() == 0) {
        return Step(-1, "", "");
    }

    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, cur.size() - 1);

    return cur[dist(rd)];
}

Step randomDefense(Bisimulation bisimulation, Step attack, std::vector<Bisimulation> history) {
    Net first = bisimulation.first;
    Net second = bisimulation.second;
    int transNum;
    std::string resNetName;
    if (attack.netName == first.name) {
        transNum = second.findRandomAvailableTransition(attack.transitionName);
        resNetName = second.name;
    }
    else {
        transNum = first.findRandomAvailableTransition(attack.transitionName);
        resNetName = first.name;
    }

    if (transNum == -1) {
        return Step(-1, "", "");
    }
    else {
        return Step(transNum, attack.transitionName, resNetName);
    }
}

Step manualAttack(Bisimulation bisimulation, std::vector<Bisimulation> history) {
    std::cout << "Choose one of transitions:" << std::endl;
    bisimulation.printAvalibleTransitions();
    int pos;
    std::string netName, transName;
    std::cin >> netName >> transName >> pos;
    Step attack(pos, transName, netName);
    return attack;
}

Step manualDefense(Bisimulation bisimulation, Step attack, std::vector<Bisimulation> history) {
    Net first = bisimulation.first;
    Net second = bisimulation.second;
    std::cout << "Choose one of transitions:" << std::endl;
    if (attack.netName == first.name) {
        second.printAllAvailableTransitions(attack.transitionName);
    }
    else {
        first.printAllAvailableTransitions(attack.transitionName);
    }
    int pos;
    std::string netName, transName;
    std::cin >> netName >> transName >> pos;
    if (transName != attack.transitionName) {
        return Step(-1, "", "");
    }
    else {
        return Step(pos, transName, netName);
    }
}

Step winAttack(Bisimulation bisimulation) {
    std::vector<Step> firstSteps = bisimulation.first.allAvailableSteps();
    std::vector<Step> secondSteps = bisimulation.second.allAvailableSteps();
    for (int i = 0; i < firstSteps.size(); ++i) {
        bool flag = false;
        for (int j = 0; j < secondSteps.size(); ++j) {
            if (firstSteps[i].transitionName == secondSteps[j].transitionName) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            return firstSteps[i];
        }
    }
    for (int j = 0; j < secondSteps.size(); ++j) {
        bool flag = false;
        for (int i = 0; i < firstSteps.size(); ++i) {
            if (firstSteps[i].transitionName == secondSteps[j].transitionName) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            return secondSteps[j];
        }
    }
    return Step(-1, "", "");
}

std::vector<std::pair<std::pair<Step, Step>, Bisimulation>> allWays(Bisimulation bisimulation) {
    std::vector<std::pair<std::pair<Step, Step>, Bisimulation>> res;
    std::vector<Step> firstSteps = bisimulation.first.allAvailableSteps();
    std::vector<Step> secondSteps = bisimulation.second.allAvailableSteps();
    for (int i = 0; i < firstSteps.size(); ++i) {
        for (int j = 0; j < secondSteps.size(); ++j) {
            if (firstSteps[i].transitionName == secondSteps[j].transitionName) {
                Bisimulation bisimulationCopy(bisimulation);
                bisimulationCopy.makeStep(firstSteps[i]);
                bisimulationCopy.makeStep(secondSteps[j]);
                res.push_back(std::pair<std::pair<Step, Step>, Bisimulation>\
                    (std::pair<Step, Step>(firstSteps[i], secondSteps[j]), bisimulationCopy));
            }
        }
    }
    return res;
}

Step smartAttack(Bisimulation bisimulation, std::vector<Bisimulation> history) {
    Net first = bisimulation.first;
    Net second = bisimulation.second;
    Step now = winAttack(bisimulation);
    if (now.transitionPosition != -1) {
        return now;
    }

    std::vector<std::pair<std::pair<Step, Step>, Bisimulation>> pred = allWays(bisimulation);

    for (int i = 0; i < pred.size(); ++i) {
        now = winAttack(pred[i].second);
        if (now.transitionPosition != -1) {
            int firstCount = first.countAvailableTransitions(pred[i].first.first.transitionName);
            int secondCount = second.countAvailableTransitions(pred[i].first.second.transitionName);
            if (firstCount >= secondCount) {
                return pred[i].first.first;
            }
            else {
                return pred[i].first.second;
            }
        }
    }
    return randomAttack(bisimulation, history);
}

std::vector<std::pair<std::pair<Step, Step>, Bisimulation>> ways(Bisimulation bisimulation, std::vector<std::pair<Step, Step>> toCheck) {
    std::vector<std::pair<std::pair<Step, Step>, Bisimulation>> res;
    for (int i = 0; i < toCheck.size(); ++i) {
        Bisimulation bisimulationCopy(bisimulation);
        bisimulationCopy.makeStep(toCheck[i].first);
        bisimulationCopy.makeStep(toCheck[i].second);
        res.push_back(std::pair<std::pair<Step, Step>, Bisimulation>\
            (toCheck[i], bisimulationCopy));
    }
    return res;
}

Step smartDefense(Bisimulation bisimulation, Step attack, std::vector<Bisimulation> history) {
    Net first = bisimulation.first;
    Net second = bisimulation.second;
    std::vector<Step> variants;
    if (attack.netName == first.name) {
        variants = second.allAvailableSteps(attack.transitionName);
    }
    else {
        variants = first.allAvailableSteps(attack.transitionName);
    }
    if (variants.size() == 0) {
        return Step(-1, "", "");
    }
    if (variants.size() > 1) {
        std::vector<std::pair<Step, Step>> toCheck;
        if (attack.netName == first.name) {
            for (int i = 0; i < variants.size(); ++i) {
                toCheck.push_back(std::pair<Step, Step>(attack, variants[i]));
            }
        }
        else {
            for (int i = 0; i < variants.size(); ++i) {
                toCheck.push_back(std::pair<Step, Step>(variants[i], attack));
            }
        }

        std::vector<std::pair<std::pair<Step, Step>, Bisimulation>> pred = ways(bisimulation, toCheck);

        for (int i = 0; i < pred.size(); ++i) {
            if (isInHistory(pred[i].second, history)) {
                return variants[i];
            }
            Step now = winAttack(pred[i].second);
            if (now.transitionPosition == -1) {
                return variants[i];
            }
        }
    }
    return variants[0];
}

void game(Bisimulation bisimulation, std::function<Step(Bisimulation, std::vector<Bisimulation>)> attackFunction, std::function<Step(Bisimulation, Step, std::vector<Bisimulation>)> defendFunction) {
    bisimulation.plotGraph("step0");
    int stepNum = 0;
    std::vector<Bisimulation> history;
    while (true) {
        std::cout << "attack:" << std::endl;

        Step attack = attackFunction(bisimulation, history);
        ++stepNum;
        attack.out();

        if (!bisimulation.makeStep(attack)) {
            std::cout << "Defender wins!" << std::endl;
            return;
        }

        bisimulation.plotGraph("step" + std::to_string(stepNum), attack);
        history.push_back(Bisimulation(bisimulation));

        std::cout << "defend:" << std::endl;

        Step defend = defendFunction(bisimulation, attack, history);
        ++stepNum;
        defend.out();

        if (!bisimulation.makeStep(defend)) {
            std::cout << "Attacker wins!" << std::endl;
            return;
        }

        for (size_t i = 0; i < history.size(); ++i) {
            if (bisimulation == history[i]) {
                std::cout << "cycle - Defender wins!" << std::endl;
                return;
            }
        }

        bisimulation.plotGraph("step" + std::to_string(stepNum), attack, defend);
        history.push_back(Bisimulation(bisimulation));
    }
}

int main()
{
    Net first("first");
    std::vector<std::string> placesNames = { "A", "B", "C", "D", "E"};
    std::vector<bool> markup = { true, true, true, false, false};
    first.setPlaces(placesNames, markup);
    std::vector<std::string> transitionsNames = { "a", "b", "c", "d"};
    std::vector<std::vector<int>> input = {\
    {0}, \
    {0}, \
    {1, 2}, \
    {2} };
    std::vector<std::vector<int>> output = {\
    {1}, \
    {2}, \
    {3}, \
    {3, 4} };
    first.setTransitions(transitionsNames, input, output);

    Net second("second");
    placesNames = { "F", "G", "H", "I", "J" };
    markup = { true, true, true, false, false };
    second.setPlaces(placesNames, markup);
    transitionsNames = { "a", "b", "c", "d"};
    input = {\
    {0}, \
    {0}, \
    {1, 2}, \
    {2} };
    output = { \
    {1}, \
    {2}, \
    {3}, \
    {3, 4} };
    second.setTransitions(transitionsNames, input, output);
    
    Bisimulation nice("nice", first, second);
    game(nice, smartAttack, smartDefense);
}
