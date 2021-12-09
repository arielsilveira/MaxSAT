#include <bits/stdc++.h>

using namespace std;

random_device g_rd;
mt19937 g_e(g_rd());
uniform_real_distribution<> g_dist_float(0, 1);

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> g_dist_int(0,1);

void printMatrix(vector<bool> matrix){
    for(int i = 0; i < matrix.size(); i++){
        cout << " " << matrix[i] << " ";
    }
    cout << endl;

}

int objectiveFunction(vector<vector<int> > maxSat, vector<bool> atoms) {
    int objectiveFunction = 0;
    int count_clause = 0;

    for(int i = 0; i < maxSat.size(); i++){
        bool clause = false;

        for(int j = 0; j < maxSat[i].size(); j++){
            
            if(maxSat[i][j] < 0){
                clause = clause || !atoms[abs(maxSat[i][j]) - 1];
            }else{
                clause = clause || atoms[maxSat[i][j] - 1];
            }

        }

        if(clause) {
            count_clause++;
        }
    }

    return count_clause;
}

int randomNeighbor(vector<vector<int> > maxSat, vector<bool> atoms, int &i, int &j) {
	float delta1, delta2;

    auto dist_int = [](int min, int max) -> double {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> g_dist(min, max);
    
        return g_dist(g_e);
    };

	j = dist_int(0, atoms.size());
	i = dist_int(0, atoms.size());
	while (i == j) {
		i = dist_int(0, atoms.size());
	}

    bool aux = atoms[i];
	atoms[i] = atoms[j];
    atoms[j] = !aux;

	return objectiveFunction(maxSat, atoms);
}

int simulatedAnnealing(vector<vector<int> > maxSat, vector<bool> atoms, float alpha, float SAmax, double temp_inicial, double temp_final) {
    vector<bool> atomsStar(atoms.size(), false);
    atomsStar = atoms;

    int fo, fo_viz, fo_star;
    int melhor_i, melhor_j;
    bool aux;
    float delta, x;
    int iterT = 0;

    double temp = temp_inicial;

    fo = fo_star = fo_viz = objectiveFunction(maxSat, atoms);

    while(temp > temp_final) {
        while(iterT < SAmax) {
            iterT++;
            fo_viz = randomNeighbor(maxSat, atoms, melhor_i, melhor_j);

            
            if(fo_viz > fo){
                fo = fo_viz;
                aux = atoms[melhor_i];
                atoms[melhor_i] = atoms[melhor_j];
                atoms[melhor_j] = !aux;

                if(fo_viz > fo_star){
                    atomsStar = atoms;
                    fo_star = fo_viz;
                }

            }else{
                x = g_dist_float(g_e);
                delta = fo_viz - fo;

                if(x < exp((-abs(delta))/temp)){
                    aux = atoms[melhor_i];
                    atoms[melhor_i] = atoms[melhor_j];
                    atoms[melhor_j] = !aux;
                    fo = fo_viz;
                }
            }
        }

        temp = alpha * temp;
        iterT = 0;
    }

    atoms = atomsStar;

    return fo_star;
}

vector<bool> randomSolutionConstruct(int size) {
    vector<bool> atoms(size);
    auto dist_int = [](int min, int max) -> double {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> g_dist(min, max);
    
        return g_dist(g_e);
    };
    
    for(int i = 0; i < size; i++){
        atoms[i] = dist_int(0, 1);
    }

    return atoms;

}

int DescentFirstImprovement(vector<vector<int> > maxSat, vector<bool> &atoms) {
	bool best  = false;
	int iter  = 0;
	int best_i  = 0;
	int best_j  = 0;

	int fo = objectiveFunction(maxSat, atoms);
	int fo_neighbor = fo;

	while(!best && iter < 500) {
		fo_neighbor = randomNeighbor(maxSat, atoms, best_i, best_j);

		if(fo_neighbor < fo) {
            bool aux = atoms[best_i];
            atoms[best_i] = atoms[best_j];
            atoms[best_j] = !aux;
			fo = fo_neighbor;
			best = true;
		}

		iter++;
	}
    fo = objectiveFunction(maxSat, atoms);
	return fo;
}

int multiStart(vector<vector<int> > maxSat, vector<bool> atoms, int iterMax) {
    float fo_star, fo;
    int iter = 0;

    vector<bool> atomsAux;
    vector<bool> atomsStar(atoms.size(), false);
    atomsStar = atoms;

    fo_star = INT_MIN;

    while(iter < iterMax){
        atomsAux = randomSolutionConstruct(atoms.size());
        fo = DescentFirstImprovement(maxSat, atomsAux);

        if(fo > fo_star){
            atomsStar = atomsAux;
            fo_star = fo;
            iter = 0;
        }else{
           iter++;
        }
    }
    
    atoms = atomsStar;

    return fo_star;
}

void perturbacao(vector<bool> &s1, int nivel){

    int nmodificacoes = nivel + 1;
    int cont = 1;
    
    int aux;
    
    while(cont <= nmodificacoes){
    
        int pos1, pos2;
        pos1 = rand() % s1.size();
        pos2 = rand() % s1.size();
        
        aux = s1[pos1];
        s1[pos1] = s1[pos2];
        s1[pos2] = aux;
        
        cont++;
    }
}

float ILS(vector<vector<int> > maxSat, vector<bool> atoms, int ILSmax, int vezesMax){
    
    DescentFirstImprovement(maxSat, atoms);
    
    int iter = 0;
    int nivel = 1;
    int nvezes = 1;
    
    while(iter < ILSmax){
    
        iter++;
        
        vector<bool> s1(atoms.size());
        s1 = atoms;
        perturbacao(s1, nivel);
        
        vector<bool> s2(atoms.size());
        s2 = s1;
        int fo_s2 = DescentFirstImprovement(maxSat, s2);
        
        if(fo_s2 > objectiveFunction(maxSat, atoms)){
            atoms = s2;
            iter = 0;
            nivel = 1;
            nvezes = 1;
       
        }else{
            if(nvezes >= vezesMax){
                nivel++;
                nvezes = 1;
 
            }else{
                nvezes++;
            }
        }
        
    }
    
    return objectiveFunction(maxSat, atoms);
}

float initialTemperature(vector<vector<int> > maxSat, vector<bool> &atoms, float beta, float gama, float SAmax, double temp_inicial) {
    double temp = temp_inicial;
    int melhor_i, melhor_j;
    bool continua = true;
    float fo_viz, fo, fo_star;
    int aceitos;
    double delta;
    double x;

    fo = fo_star = fo_viz = objectiveFunction(maxSat, atoms);

    while(continua) {
        aceitos = 0;
        for (int iterT = 1; iterT <= SAmax; iterT++) {
            fo_viz = randomNeighbor(maxSat, atoms, melhor_i, melhor_j);
            delta = fo_viz - fo;
            if (delta < 0) {
                aceitos++;
            }else{
                x = g_dist_float(g_e);
                if ( x < exp(-delta/temp)){
                    aceitos++;
                }
            }
        }

        if(aceitos >= gama * SAmax){
            continua = false;
        }else{
            temp = beta * temp;
        }
    }

    return temp;
}

void readFile(string name, vector<vector<int> > &maxSat, vector<bool> &atoms) {
    int count_atoms, count_clauses;
    string line, waste;
    int atom1, atom2, atom3;

    string path = "input/" + name;

	ifstream file;
	
    file.open(path);

	if(!file.is_open()){
		cout << "Fail to open file" << endl;
		exit(1);
	}

	for (int i = 1; i <= 7; ++i){
		getline(file, line);
	}

    file >> waste >> waste >> count_atoms >> count_clauses;

    for(int i = 0; i < count_atoms; i++) {
        atoms.push_back(false);
    }


    for(int i = 0; i < count_clauses; i++) {
        maxSat.push_back(vector<int>());

        file >> atom1 >> atom2 >> atom3 >> waste;
        maxSat[i].push_back(atom1);
        maxSat[i].push_back(atom2);
        maxSat[i].push_back(atom3);
    }
}

int main(int argc, const char *argv[]){
    vector<vector<int> > maxSat;
    vector<bool> atoms;

    if(argc != 2){
		cout << "./a.out file_name" << endl;
		exit(0);
	}
    
    readFile(argv[1], maxSat, atoms);

    cout << "Count MaxSAT: " << maxSat.size() << endl;
    cout << "Count atoms: " << atoms.size() << endl;

    cout << "Objective Function Initial: " << objectiveFunction(maxSat, atoms) << endl;
    clock_t start, end;
    double time_taken;

    start = clock();
    int temp = initialTemperature(maxSat, atoms, 2, 0.95, 4*atoms.size(), 10);
    int objFunction = simulatedAnnealing(maxSat, atoms, 0.98, 20*atoms.size(), temp, 0.01);
    end = clock();
    cout << "Objective Function Simulated Annealing: " << objFunction << endl;
    time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time execution: " << time_taken << " s" << endl; 
    
    start = clock();
    cout << "Objective Function MultiStart: " << multiStart(maxSat, atoms, 50000) << endl;
    end = clock();
    time_taken = double(end - start) / double(CLOCKS_PER_SEC);

    cout << "Time execution: " << time_taken << " s" << endl; 

    start = clock();
    cout << "Objective Function ILS: " << ILS(maxSat, atoms, 50000, 50) << endl;
    end = clock();
    time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time execution: " << time_taken << " s" << endl; 

    return 0;
}