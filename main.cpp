#include <iostream>

using namespace std;

// Se manda a llamar a los demás archivos
#include "geometry/mesh.hpp"
#include "math_utilities/matrix_operations.hpp"
#include "mef_utilities/mef_process.hpp"
#include "gid/input_output.hpp"

// Argumentos para garantizar el formato correcto del archivo a recibir
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Incorrect use of the program, it must be: mef filename\n";
        exit(EXIT_FAILURE);
    }

    // Declaración de la malla
    Mesh M;

    // Se leen los datos de la malla
    cout << "Reading geometry and mesh data...\n\n";
    string filename(argv[1]);
    read_input(filename, &M);
    // M.report();

    // Se declara el numero de nodos a obtener
    short num_nodes = M.get_quantity(NUM_NODES);
    // Se declara el numero de elementos a obtener
    short num_elements = M.get_quantity(NUM_ELEMENTS);
    Matrix K(num_nodes, num_nodes), local_Ks[num_elements];
    Vector b(num_nodes), local_bs[num_elements];

    // Se manda a llamar a la función que crea los sistemas locales
    cout << "Creating local systems...\n\n";
    create_local_systems(local_Ks, local_bs, num_elements, &M);

    // Se manda a llamar a la función que realiza el ensamblaje
    cout << "Performing Assembly...\n\n";
    assembly(&K, &b, local_Ks, local_bs, num_elements, &M);

    // K.show(); b.show();

    // Se manda a llamar a la función que aplica las condiciones de Neumann
    cout << "Applying Neumann Boundary Conditions...\n\n";
    apply_neumann_boundary_conditions(&b, &M);

    // b.show();

    // Se manda a llamar a la función que aplica las condiciones de Dirichlet
    cout << "Applying Dirichlet Boundary Conditions...\n\n";
    apply_dirichlet_boundary_conditions(&K, &b, &M);

    // K.show(); b.show();

    // Se manda a llamar a la función que resuelve el sistema global
    cout << "Solving global system...\n\n";
    Vector T(b.get_size()), T_full(num_nodes);
    solve_system(&K, &b, &T);
    // T.show();

    // Se manda a llamar a la función que prepara los resultados para mostrarlos de forma coherente
    cout << "Preparing results...\n\n";
    merge_results_with_dirichlet(&T, &T_full, num_nodes, &M);
    // T_full.show();

    // Se manda a llamar a la función que escribe el archivo pos.res
    cout << "Writing output file...\n\n";
    write_output(filename, &T_full);

    return 0;
}
