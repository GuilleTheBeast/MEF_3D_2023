#include <fstream>

// Función para leer el archivo ingresado
void read_input(string filename, Mesh *M)
{
    string line;
    float k, Q, T_bar, T_hat;
    int num_nodes, num_elements, num_dirichlet, num_neumann;
    ifstream dat_file(filename + ".dat");

    // Datos que espera recibir la funcion del archivo .dat
    dat_file >> k >> Q >> T_bar >> T_hat >> num_nodes >> num_elements >> num_dirichlet >> num_neumann;

    M->set_problem_data(k, Q);
    M->set_quantities(num_nodes, num_elements, num_dirichlet, num_neumann);

    M->init_arrays();

    dat_file >> line;

    for (int i = 0; i < num_nodes; i++)
    {
        int id;
        float x, y, z;
        dat_file >> id >> x >> y >> z;
        M->insert_node(new Node(id, x, y, z), i);
    }

    dat_file >> line >> line;

    for (int i = 0; i < num_elements; i++)
    {
        int id, node1_id, node2_id, node3_id, node4_id;
        dat_file >> id >> node1_id >> node2_id >> node3_id >> node4_id;
        M->insert_element(new Element(id, M->get_node(node1_id - 1), M->get_node(node2_id - 1), M->get_node(node3_id - 1), M->get_node(node4_id - 1)), i);
    }

    dat_file >> line >> line;

    for (int i = 0; i < num_dirichlet; i++)
    {
        int id;
        dat_file >> id;
        M->insert_dirichlet_condition(new Condition(M->get_node(id - 1), T_bar), i);
    }

    dat_file >> line >> line;

    for (int i = 0; i < num_neumann; i++)
    {
        int id;
        dat_file >> id;
        M->insert_neumann_condition(new Condition(M->get_node(id - 1), T_hat), i);
    }

    dat_file.close();
}

// Función para crear el archivo post.res
void write_output(string filename, Vector *T)
{
    // nombre del archivo generado
    ofstream res_file(filename + ".post.res");
    // Encabezado del archivo
    res_file << "GiD Post Results File 1.0\n";

    int n = T->get_size();

    res_file << "Result \"Temperature\" \"Load Case 1\" " << 1 << " Scalar OnNodes\n";
    res_file << "ComponentNames \"T\"\n";
    res_file << "Values\n";

    for (int i = 0; i < n; i++)
        res_file << i + 1 << "     " << T->get(i) << "\n";

    res_file << "End values\n";

    res_file.close();
}
