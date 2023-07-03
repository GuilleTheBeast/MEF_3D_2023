/*Se declara la clase "Condition" donde se crean las variables en que se almacenaran los nodos a los que se les
apliquen las condiciones de contorno*/
class Condition
{
private:
    Node *node;
    float value;

public:
    Condition(Node *node_to_assign, float value_to_assign)
    {
        node = node_to_assign;
        value = value_to_assign;
    }

    // Setters y getters de los nodos y valores declarados
    void set_node(Node *node_to_assign)
    {
        node = node_to_assign;
    }
    Node *get_node()
    {
        return node;
    }

    void set_value(float value_to_assign)
    {
        value = value_to_assign;
    }
    float get_value()
    {
        return value;
    }
};
