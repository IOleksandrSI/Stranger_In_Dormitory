#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <ctime>
using namespace sf;
// настройки тимчасових елементів гри
const int step_color = 2.0; // Крок кольору
const int step_sequense = 12; // Кількість кроків для збільшення прискорення
const float min_score_enemy = 5; // Мінімальна кількість життів гравця
const float angle_bootl_rotate = 1.0; // кількість градусів повороту за 1 цикл
const float step = 1.5; // швидкість гравці
const float sequense_speed = 0.15; // швидкість прискорення
const float max_speed = 7; // максимальна швидкість
const float step_enemy = step / 2; // швидкість ворога
const float distanse_step_enemy = 2.7; // ввідстань від 1 точки до іншої в блоках

// настройки постійних елементів гри
const int time_out = 120; // Кількість часу скільки йде гра
const int min_distance_with_player_bloc = 2; // Мінімальна відстань між гравцем і іншими об'єктами
const float relation_points = 5.0; // Різниця між oб'єктами
const float relation_player_score = 1.00; // коефіцієнт спаду очок за 1 секунду
const float relation_cels_bootls = 0.20; // Процент бутилок на карті по відношенню до вільних клітинок
const float relation_cels_enemys = 0.066; // Процент ворогів на карті по відношенню до вільних клітинок
const float time_life_bootl = 20.0; // кількість секунд скільки живе бутилка
const float relation_collision_die = 0.4; // коефіцієнт для зрізу при зіткненні з ворохом, при смерті
const float relation_collision_kill = 0.1; // коефіцієнт для добавлення при зіткненні з ворого, при вбивстві

// розміри 
const int padding_x = 5; // Бокові відступи
const int padding_y = 5; // Вертикальні відступи
const int bloc_size = 60; // Розмір клітинки
const int size_matrix_w = 15; // Кількість клітинок по горизонталі
const int size_matrix_h = 11; // Кількість клітинок по вертикалі
const int W = bloc_size * size_matrix_w; // Ширина вікна
const int H = bloc_size * size_matrix_h; // Висота вікна
const int size_info_bar = 70 + padding_y; // розмір інфо бару
const int line_rectangle_info_bar_size = 1; //
const int padding_info_bar_x = 10; //
const int padding_info_bar_y = 5; //
const int size_font_timer = size_info_bar - padding_info_bar_y * 2; //
const int size_font_scores = size_info_bar - padding_info_bar_y * 2; //
const int size_font_name = size_font_scores*0.75; //
const int size_enemy_font = 17; // розміри шрифта в прямокутник ворога
const int padding_rectangle_enemy_y = 2; // відступ в середині по вертикалі
const int padding_rectangle_enemy_x = 5; // відступ в середині по горизонталі
const int outline_rectangle_enemy = 1; // обводка прямокутника ворога
const float fault_x = padding_x; // поправка по горизонталі
const float fault_y = padding_y + size_info_bar; // поправка по вертикалі
const float radius_enity = 22.5; // радіус персонажів
const float radius_bootls = 22.5; // радіус бонусів
const float radius_line_bootl = 2.5; // радіус контурної ліній

const std::string RobotoBlack = "fonts\\Roboto-Black.ttf";
const std::string RobotoMedium = "fonts\\Roboto-Medium.ttf";
// Інфобар

// Colors
const Color line_rectangle_enemy_color = Color::White;
const Color rectangle_enemy_color = Color(128, 128, 128);
const Color font_rectangle_enemy_color = Color::Red;
const Color font_info_bar_color = Color::Red;
const Color rectangle_info_bar_color = Color::White;
const Color line_rectangle_info_bar_color = Color::Red;


long double pi = 3.14159265358979323851;
const double RADTODEGREE = 180 / pi;



struct Point
{
    float x, y;
    Point(float _x = 0, float _y = 0)
    {
        x = _x;
        y = _y;
    }
};

struct PointInt
{
    int x, y;
    PointInt(int _x = 0, int _y = 0)
    {
        x = _x;
        y = _y;
    }
};
float distanse(Point p1, Point p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
float distanse(PointInt p1, PointInt p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
int random_int(int a, int b)
{
    int c = b - a;
    return rand() % c + a;
}

class Animation
{
public:
    float Frame, speed;
    Sprite sprite;
    std::vector<IntRect> frames;

    Animation() {}

    Animation(Texture& t, int x, int y, int w, int h, int count, float Speed)
    {
        set(t, x, y, w, h, count, Speed);
    }
    void set(Texture& t, int x, int y, int w, int h, int count, float Speed)
    {
        Frame = 0;
        speed = Speed;

        for (int i = 0; i < count; i++)
            frames.push_back(IntRect(x + i * w, y, w, h));

        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(frames[0]);
    }


    void update()
    {
        Frame += speed;
        int n = frames.size();
        if (Frame >= n) Frame -= n;
        if (n > 0) sprite.setTextureRect(frames[int(Frame)]);
    }

    bool isEnd()
    {
        return Frame + speed >= frames.size();
    }

};

class BootlType
{
public:
    Texture t;
    float bonus = 0;
    int type;
    std::string name;
    BootlType(){}
    void InitFromFile(std::string skin_file, float _bonus, int _type)
    {
        t.loadFromFile(skin_file);
        bonus = _bonus;
        int s = skin_file.find('\\') + 1, e = skin_file.rfind('.');
        name = skin_file.substr(s, e);
        type = _type;
    }
};
class Bootl
{
private:
    Texture *t;
    CircleShape btl;
    std::string name;
    Point p, c;
    PointInt p_b;
    int type = 0;
    int stage_outline = 1;
    int r = 255, g = 0, b = 0;
    float radius;
    float bonus = 0;
    float angle_rotate = angle_bootl_rotate;
    float step_life = 1;
    bool life = true;
public:
    Bootl(BootlType& other)
    {
        set();
        t = &(other.t);
        bonus = other.bonus;
        name = other.name;
        type = other.type;
        InitFromFiles();
    }
    Bootl(float _x = 0, float _y = 0, float c_x = radius_bootls, float c_y = radius_bootls, float _radius = radius_bootls)
    {
        set(_x, _y, c_x, c_y, _radius);
    }
    ~Bootl() { }
    void set(float _x = 0, float _y = 0, float c_x = radius_bootls, float c_y = radius_bootls, float _radius = radius_bootls)
    {
        p.x = _x + radius;
        p.y = _y + radius;
        c.x = c_x;
        c.y = c_y;
        radius = _radius;
    }
    void InitFromFiles() {
        btl.setTexture(t);
        btl.setRadius(radius);
        btl.setOutlineThickness(radius_line_bootl);
        btl.setOutlineColor(Color(r, g, b));
        btl.setOrigin(c.x, c.y);
    }
    void setName(std::string _s)
    {
        name = _s;
    }
    void setCoord(float _x, float _y)
    {
        p.x = _x;
        p.y = _y;
    }
    void setCoordBloc(int _x, int _y)
    {
        p_b.x = _x;
        p_b.y = _y;
    }
    void setBonus(float _b)
    {
        bonus = _b;
        step_life = _b / time_life_bootl;
    }

    void updateCoords()
    {
        btl.setPosition(p.x + fault_x, p.y + fault_y);
    }
    void update_milisecond()
    {
        btl.rotate(angle_rotate);
        switch (stage_outline)
        {
        case(1):
            b += step_color;
            if (b >= 255) { b = 255; stage_outline++; }
            break;
        case(2):
            r -= step_color;
            if (r <= 0) { r = 0; stage_outline++; }
            break;
        case(3):
            g += step_color;
            if (g >= 255) { g = 255; stage_outline++; }
            break;
        case(4):
            b -= step_color;
            if (b <= 0) { b = 0; stage_outline++; }
            break;
        case(5):
            r += step_color;
            if (r >= 255) { r = 255; stage_outline++; }
            break;
        case(6):
            g -= step_color;
            if (g <= 0) { g = 0; stage_outline = 1; }
            break;          
        };
        btl.setOutlineColor(Color(r, g, b));

    }
    void draw(RenderWindow& app)
    {
        app.draw(btl);
    }

    bool update()
    {
        bonus -= step_life;
        if (bonus <= 0)
        {
            return false;
        }
        return true;
    }

    float getRadius() { return radius; }
    float getScore() { return bonus; }
    int getBlocX(){ return p_b.x; }
    int getBlocY(){ return p_b.y; }
    Point getPos() { return p; }
    int getType() { return type; }
};

class EnemyType
{
public:
    Texture t;
    int strenght = 0;
    int type = 0;
    Font f;
    std::string name;
    EnemyType() {}
    void InitFromFile(std::string enemy_file, float _strenght, int _type)
    {
        t.loadFromFile(enemy_file);
        strenght = _strenght;
        type = _type;
        if (!f.loadFromFile(RobotoMedium)) { std::cout << "!!!!!!!!" << std::endl; }
        
        int s = enemy_file.find('\\') + 1, e = enemy_file.rfind('.');
        name = enemy_file.substr(s, e);
    }
};
class Enemy {
private:
    Point p, c, p_rec, text;
    float radius;
    int score;
    Texture* t;
    Animation enm;
    RectangleShape rec;
    Text strenght;
    Font* f;
    std::string name;
    int type = 0;

    bool is_run = false, directio = true;
    int in_steps = 0;
    bool update_steps = true;
    Point d;
    float angle = 0;
    std::vector <Point> way;
public:
    Enemy(EnemyType& other, float player_score)
    {
        set();
        t = &(other.t);
        f = &(other.f);
        score = min_score_enemy + player_score * other.strenght;
        name = other.name;
        type = other.type;
        angle = 0;
        InitFromFiles();
    }
    Enemy(float _x = 0, float _y = 0, float _c_x = radius_enity, float _c_y = radius_enity, float _radius = radius_enity)
    {
        set(_x, _y, _c_x, _c_y, _radius);
    }
    ~Enemy() { }
    void set(float _x = 0, float _y = 0, float _c_x = radius_enity, float _c_y = radius_enity, float _radius = radius_enity)
    {
        p.x = _x + _radius;
        p.y = _y + _radius;
        c.x = _c_x;
        c.y = _c_y;
        radius = _radius;
    }
    void InitFromFiles() {
        enm.set(*t, 0, 0, 45, 45, 8, 0.05);
        std::string temp = std::to_string(int(score));

        String sss(temp);
       
        strenght.setString(sss);
        strenght.setFont(*f);
        strenght.setFillColor(font_rectangle_enemy_color);
        strenght.setCharacterSize(size_enemy_font);

        float st_x, st_y;
        FloatRect bounds(strenght.getLocalBounds());
        st_x = bounds.width / 2;
        st_y = bounds.height - 1;
        strenght.setOrigin(st_x, st_y);
        float r_w = padding_rectangle_enemy_x * 2 + outline_rectangle_enemy * 2 + bounds.width, r_h = padding_rectangle_enemy_y * 2 + outline_rectangle_enemy * 2 + bounds.height;
        rec.setSize(Vector2f(r_w, r_h));
        rec.setOrigin(Vector2f(r_w / 2, r_h - 1));
        rec.setOutlineThickness(outline_rectangle_enemy);
        rec.setOutlineColor(line_rectangle_enemy_color);
        rec.setFillColor(rectangle_enemy_color);
        text.x = 0;
        text.y = padding_rectangle_enemy_y *2 + outline_rectangle_enemy + radius + 5;
        p_rec.y = radius + 1;
        p_rec.x = 0;
    }
    void updateCoords()
    {
        enm.sprite.setPosition(p.x + fault_x, p.y + fault_y);
        rec.setPosition(p.x + fault_x - p_rec.x, p.y + fault_y - p_rec.y);
        strenght.setPosition(p.x + fault_x - text.x, p.y + fault_y - text.y);
    }
    void updateDirection()
    {
        if (update_steps && is_run)
        {
            if (directio == true)
            {
                in_steps++;
                if (in_steps == way.size() - 1) { directio = false; }
            }
            else
            {
                in_steps--;
                if (in_steps == 0) { directio = true; }
            }

            d.x = 0;
            d.y = 0;
            float all_x = way[in_steps].x - p.x, all_y = way[in_steps].y - p.y;
            if (all_x > 0) { d.x = step_enemy; angle = 270; }
            else if (all_x < 0) { d.x = -1 * step_enemy; angle = 90; }
            else if (all_y > 0) { d.y = step_enemy; angle = 0; }
            else if (all_y < 0) { d.y = -1 * step_enemy; angle = 180; }
            
            updateRotation();
            update_steps = false;
        }
    }
    void updateRotation()
    {
        
        enm.sprite.setRotation(angle);
    }
    bool checkUpdate()
    {
        if (is_run == false) { return false; }
        if (p.x >= 440)
        {
            p.x = p.x;
        }
        float _d = distanse(p, way[in_steps]);
        
        
        if (_d <= abs(step_enemy))
        {
            float all_x = way[in_steps].x - p.x, all_y = way[in_steps].y - p.y;
            if (all_x > 0) { d.x = _d; }
            else if (all_x < 0) { d.x = -1 * _d; }
            else if (all_y > 0) { d.y = _d; }
            else if (all_y < 0) { d.y = -1 * _d; }
            update_steps = true;
        }
        return true;
    }
    void update()
    {
        p.y += d.y;
        p.x += d.x;
        updateCoords();
    }
    void setWay(std::vector<PointInt> _way)
    {
        if (_way.size() != 0)
        {
            for (int i = 0; i < _way.size(); i++)
            {
                way.push_back(Point((_way[i].x * bloc_size + bloc_size / 2), _way[i].y * bloc_size + bloc_size / 2));
            }
            is_run = true;
        }
    }
    void draw(RenderWindow& app)
    {
        enm.update();
        app.draw(enm.sprite);
        app.draw(rec);
        app.draw(strenght);
    }
    void draw_else(RenderWindow& app)
    {
        app.draw(enm.sprite);
    }

    float getRadius()
    {
        return radius;
    }
    Point getPos()
    {
        return p;
    }
    void setCoord(float _x, float _y)
    {
        p.x = _x;
        p.y = _y;
    }
    Point getPointD() { return d; }
    int getBlocX() { return p.x / bloc_size; }
    int getBlocY() { return p.y / bloc_size; }
    float getScore() { return score; }
    int getType() { return type; }
};

class Background {
private:
    std::vector <char> MiniMap;
    std::vector <PointInt> FreeCellsList;
    std::vector <char> Map;
    std::vector <Sprite> TextureMap;
    Texture t_floor, t_wall;
    int free_cells = 0;
public:
    Background(){}
    Background(std::vector <char>& M) :MiniMap(M)
    {}
    void LoadTextures(std::string floor_file, std::string wall_file)
    {
        t_floor.loadFromFile(floor_file);
        t_wall.loadFromFile(wall_file);


    }
    void RefreshTextureMap()
    {
        TextureMap.clear();
        free_cells = 0;
        for (int y = 0; y < size_matrix_h; y++) {
            for (int x = 0; x < size_matrix_w; x++)
            {
                Sprite s;
                TextureMap.push_back(s);
                if (MiniMap[y * size_matrix_w + x] == '-') {
                    TextureMap[y * size_matrix_w + x].setTexture(t_floor);
                    free_cells++;
                    FreeCellsList.push_back(PointInt(x, y));
                }
                else {
                    TextureMap[y * size_matrix_w + x].setTexture(t_wall);
                }
                TextureMap[y * size_matrix_w + x].setPosition(x * bloc_size + fault_x, y * bloc_size + fault_y);
            }
        }
    }
    void genetateFullMap()
    {
        for (int y = 0; y < H; y++)
        {
            for (int x = 0; x < W; x++)
            {
                if (MiniMap[(y / bloc_size) * size_matrix_w + (x / bloc_size)] == '-')
                {
                    Map.push_back('-');
                }
                else
                {
                    Map.push_back('#');
                }
            }
        }
    }
    void LoadMap(std::vector <char>& M)
    {
        MiniMap.clear();
        MiniMap = M;
    }

    int getFreeCells() { return free_cells; }

    
    bool CountMoved(Point _p, float radius, Point p_step)
    {
        Point p;
        PointInt p_bloc;
        p.x = _p.x + p_step.x;
        p.y = _p.y + p_step.y;

        p_bloc.x = p.x / bloc_size;
        p_bloc.y = p.y / bloc_size;
        if (p.y + radius + 1 >= H)
        {
            return false;
        }
        float arr[8] = { (p.x - radius) - p_bloc.x * bloc_size, (p_bloc.x + 1) * bloc_size - (p.x + radius), (p.y - radius)  - p_bloc.y * bloc_size, (p_bloc.y + 1) * bloc_size - (p.y + radius),
            distanse(Point(p_bloc.x * bloc_size, p_bloc.y * bloc_size), p) - radius, distanse(Point(p_bloc.x * bloc_size, (p_bloc.y + 1) * bloc_size), p) - radius,
            distanse(Point((p_bloc.x + 1) * bloc_size, p_bloc.y * bloc_size), p) - radius, distanse(Point((p_bloc.x + 1) * bloc_size, (p_bloc.y + 1) * bloc_size), p) - radius, };
        if (arr[0] < 0 && (p_bloc.x == 0 || MiniMap[p_bloc.y * size_matrix_w + p_bloc.x - 1] == '#'))
        {
            return false;
        } else if (arr[1] < 0 && (p_bloc.x == size_matrix_w - 1 || MiniMap[p_bloc.y * size_matrix_w + p_bloc.x + 1] == '#'))
        {
            return false;
        }
        if (arr[2] < 0 && (p_bloc.y == 0 || MiniMap[(p_bloc.y - 1) * size_matrix_w + p_bloc.x] == '#'))
        {
            return false;
        }else if (arr[3] < 0 && (p_bloc.y == size_matrix_w - 1 || MiniMap[(p_bloc.y + 1) * size_matrix_w + p_bloc.x] == '#'))
        {
            return false;
        }
        if (arr[4] < 0 && MiniMap[(p_bloc.y - 1) * size_matrix_w + p_bloc.x - 1] == '#')
        {
            return false;
        }
        else if (arr[6] < 0 && MiniMap[(p_bloc.y - 1) * size_matrix_w + p_bloc.x + 1] == '#')
        {
            return false;
        }
        else if (arr[7] < 0 && MiniMap[(p_bloc.y + 1) * size_matrix_w + p_bloc.x + 1] == '#')
        {
            return false;
        }
        else if (arr[5] < 0 && MiniMap[(p_bloc.y + 1) * size_matrix_w + p_bloc.x - 1] == '#')
        {
            return false;
        }
        return true;
    }



    PointInt getRandonmFreeBloc(Point _p, float r1, float r2, std::vector <Bootl> &lst_btl, int count)
    {
        PointInt btl_bloc, p_bloc;
        p_bloc.x = _p.x / bloc_size;
        p_bloc.y = _p.y / bloc_size;
        bool checker = true;
        while (true)
        {
            int rand_index_bloc = rand() % (FreeCellsList.size());
            btl_bloc = FreeCellsList[rand_index_bloc];
            float d = distanse(btl_bloc, p_bloc);
            if (d < min_distance_with_player_bloc)
            {
                continue;
            }
            checker = true;
            for (int i = 0; i < count; i++)
            {
                if (lst_btl[i].getBlocX() == btl_bloc.x && lst_btl[i].getBlocY() == btl_bloc.y)
                {
                    checker = false;
                    break;
                }
            }
            if (checker)
            {
                return btl_bloc;
            }
        }
    }
    PointInt getRandonmFreeBloc(Point _p, float r1, float r2, std::vector <Enemy> &lst_enm, int count)
    {
        PointInt enm_bloc, p_bloc;
        p_bloc.x = _p.x / bloc_size;
        p_bloc.y = _p.y / bloc_size;
        bool checker = true;
        while (true)
        {
            int rand_index_bloc = rand() % (FreeCellsList.size());
            enm_bloc = FreeCellsList[rand_index_bloc];
            float d = distanse(enm_bloc, p_bloc);
            if (d < min_distance_with_player_bloc)
            {
                continue;
            }
            checker = true;
            for (int i = 0; i < count; i++)
            {
                if (lst_enm[i].getBlocX() - enm_bloc.x < 1 && lst_enm[i].getBlocY() - enm_bloc.y < 1)
                {
                    checker = false;
                    break;
                }
            }
            if (checker)
            {
                return enm_bloc;
            }
        }
    }
    PointInt getRandonmFreeBloc(std::vector <Enemy>& lst_enm, int count)
    {
        PointInt player_bloc;
        bool checker = true;
        while (true)
        {
            int rand_index_bloc = rand() % (FreeCellsList.size());
            player_bloc = FreeCellsList[rand_index_bloc];
            checker = true;
            for (int i = 0; i < count; i++)
            {
                if (lst_enm[i].getBlocX() - player_bloc.x < 2 && lst_enm[i].getBlocY() - player_bloc.y < 2)
                {
                    checker = false;
                    break;
                }
            }
            if (checker)
            {
                return player_bloc;
            }
        }
    }

    std::vector<PointInt> getWayEnemy(Point _p, float r1, float r2, std::vector <Enemy>& lst_enm)
    {
        PointInt start = getRandonmFreeBloc(_p, r1, r2, lst_enm, lst_enm.size() - 1);
        PointInt two_th, end;
        std::vector<PointInt> way, temp;
        way.push_back(start);
        while (true)
        {
            int rand_index_bloc = rand() % (FreeCellsList.size());
            two_th = FreeCellsList[rand_index_bloc];
            float d = distanse(start, two_th);
            if (d >= distanse_step_enemy)
            {
                break;
            }
        }
        temp = search_way(start, two_th);
        if (temp.size() == 0) { return way; }
        for (int i = temp.size() - 1; i >= 0 ; i--) { way.push_back(temp[i]); }
        temp.clear();
        while (true)
        {
            int rand_index_bloc = rand() % (FreeCellsList.size());
            two_th = FreeCellsList[rand_index_bloc];
            float d = distanse(start, two_th);
            if (d >= distanse_step_enemy)
            {
                break;
            }
        }
        if (temp.size() == 0) { return way; }
        for (int i = temp.size() - 1; i >= 0; i--) { way.push_back(temp[i]); }
        return way;
    }

    std::vector<PointInt> search_way(PointInt start, PointInt end)
    {
        short map[size_matrix_h * size_matrix_w];
        for (int i = 0; i < size_matrix_h * size_matrix_w; i++) { map[i] = 0; }
        
        std::vector<PointInt> way;
        way.push_back(start);
        map[start.y * size_matrix_w + start.x] = 1;
        while (way.size() > 0)
        {
            PointInt p = way[0];
            way.erase(way.begin());
            if (p.x != 0 && map[p.y * size_matrix_w + p.x - 1] == 0 && MiniMap[p.y * size_matrix_w + p.x - 1] == '-')
            {
                way.push_back(PointInt(p.x - 1, p.y));
                map[p.y * size_matrix_w + p.x - 1] = map[p.y * size_matrix_w + p.x] + 1;
            }
            if (p.x != size_matrix_w - 1 && map[p.y * size_matrix_w + p.x + 1] == 0 && MiniMap[p.y * size_matrix_w + p.x + 1] == '-')
            {
                way.push_back(PointInt(p.x + 1, p.y));
                map[p.y * size_matrix_w + p.x + 1] = map[p.y * size_matrix_w + p.x] + 1;
            }
            if (p.y != 0 && map[(p.y - 1) * size_matrix_w + p.x] == 0 && MiniMap[(p.y - 1) * size_matrix_w + p.x] == '-')
            {
                way.push_back(PointInt(p.x, (p.y - 1)));
                map[(p.y - 1) * size_matrix_w + p.x] = map[p.y * size_matrix_w + p.x] + 1;
            }
            if (p.y != size_matrix_h - 1 && map[(p.y + 1) * size_matrix_w + p.x] == 0 && MiniMap[(p.y + 1) * size_matrix_w + p.x] == '-')
            {
                way.push_back(PointInt(p.x, (p.y + 1)));
                map[(p.y + 1) * size_matrix_w + p.x] = map[p.y * size_matrix_w + p.x] + 1;
            }

        }
        if (map[end.y * size_matrix_w + end.x] == 0) { return way; }
        while (map[end.y * size_matrix_w + end.x] != 1)
        {
            way.push_back(end);
            if (end.x != 0 && map[end.y * size_matrix_w + end.x - 1] == map[end.y * size_matrix_w + end.x] - 1) { end.x--; }
            else if (end.x != size_matrix_w - 1 && map[end.y * size_matrix_w + end.x + 1] == map[end.y * size_matrix_w + end.x] - 1) { end.x++; }
            else if (end.y != 0 && map[(end.y - 1) * size_matrix_w + end.x] == map[end.y * size_matrix_w + end.x] - 1) { end.y--; }
            else if (end.y != size_matrix_h - 1 && map[(end.y + 1) * size_matrix_w + end.x] == map[end.y * size_matrix_w + end.x] - 1) { end.y++; }
        }
        return way;
    }


    void draw(RenderWindow& app) {
        for (int y = 0; y < size_matrix_h; y++) {
            for (int x = 0; x < size_matrix_w; x++)
            {
                app.draw(TextureMap[y* size_matrix_w + x]);
            }
        }
    }
};


class Player {
private:
    Point p, c;
    float radius;
    float score;

    Texture t;
    Animation ent;
    int angle = 0, previos_angle = 0;
    int count_sequense = 0;
public:
    Player(float _x = 0, float _y = 0, float _c_x = radius_enity, float _c_y = radius_enity, float _radius = radius_enity)
    {
        set(_x, _y, _c_x, _c_y, _radius);
    }
    void set(float _x = 0, float _y = 0, float _c_x = radius_enity, float _c_y = radius_enity, float _radius = radius_enity)
    {
        p.x = _x + _radius;
        p.y = _y + _radius;
        c.x = _c_x;
        c.y = _c_y;
        radius = _radius;
    }
    void InitFromFiles(std::string skin_file) {
        t.loadFromFile(skin_file);
        ent.set(t, 0, 0, 45, 45, 8, 0.1);
    }
    void setCoord(float _x, float _y)
    {
        p.x = _x;
        p.y = _y;
    }
    void updateCoords()
    {
        ent.sprite.setPosition(p.x + fault_x, p.y + fault_y);
    }
    void updateRotation(float _x, float _y)
    {
        previos_angle = angle;
        if (_x < 0)
        {
            angle = 90;
        }
        if (_x > 0)
        {
            angle = 270;
        }
        if (_y > 0)
        {
            angle = 0;
        }
        if (_y < 0)
        {
            angle = 180;
        }
        ent.sprite.setRotation(angle);
        if (angle == previos_angle)
        {
            count_sequense++;
        }
        else
        {
            count_sequense = 0;
        }
    }
    bool addScore(float s)
    {
        score += s;
        if (score < 0)
        {
            score = 0;
            return false;
        }
        return true;
    }
    void Move(float _x, float _y, Background& bg, int ContSteps = 1)
    {

        bool temp;
        updateRotation(_x, _y);
        int z1 = (_x > 0)*2 - 1, z2 = (_y > 0) * 2 - 1;
        if (_y == 0) { _x = _x + z1 * (count_sequense/step_sequense) * sequense_speed; }
        else { _y = _y + z2 * (count_sequense/step_sequense) * sequense_speed; }
        if (max_speed < abs(_x)) { _x = z1*max_speed; }
        if (max_speed < abs(_y)) { _y = z2*max_speed; }
        
        temp = bg.CountMoved(p, radius, Point(_x, _y));
        if (temp)
        {
            ent.update();
            p.y += _y;
            p.x += _x;
            updateCoords();
        }
    }
    void update()
    {
        score *= relation_player_score;
    }
    void draw(RenderWindow& app)
    {
        app.draw(ent.sprite);
    }
    float getRadius()
    {
        return radius;
    }
    Point getPos()
    {
        return p;
    }
    float getScore(){return score;}
};

class InfoBar
{
private:
    Text Timer, Name, Score;
    Font RM, RB;
    Color info_color;
    int size_t = 3, size_s = 6;
    RectangleShape rec_bg;
public:
    InfoBar(){}
    void Load()
    {
        rec_bg.setSize(Vector2f(W, size_info_bar - padding_y));
        rec_bg.setOutlineThickness(line_rectangle_info_bar_size);
        rec_bg.setOutlineColor(line_rectangle_info_bar_color);
        rec_bg.setFillColor(rectangle_info_bar_color);
        rec_bg.setPosition(padding_x, padding_y);
        if (!RM.loadFromFile(RobotoMedium)) { std::cout << "!!!!!!!!" << std::endl; }
        if (!RB.loadFromFile(RobotoBlack)) { std::cout << "!!!!!!!!" << std::endl; }
        Timer.setFont(RB);
        Timer.setOrigin(0, size_font_timer / 2);
        Timer.setFillColor(font_info_bar_color);
        Timer.setCharacterSize(size_font_timer);
        Timer.setStyle(Text::Bold);
        Name.setFont(RM);
        Name.setOrigin(0, size_font_timer / 2);
        Name.setFillColor(font_info_bar_color);
        Name.setCharacterSize(size_font_name);
        Name.setStyle(Text::Italic);
        Score.setFont(RB);
        Score.setOrigin(0, size_font_timer / 2);
        Score.setFillColor(font_info_bar_color);
        Score.setCharacterSize(size_font_scores);
        Score.setStyle(Text::Bold);

        Timer.setPosition(padding_x + padding_info_bar_x, padding_y + (size_info_bar - padding_y) * 0.35);
        Name.setPosition(padding_x  + W/2, padding_y + (size_info_bar - padding_y) * 0.5);
        Score.setPosition(padding_x + (W - padding_info_bar_x - (size_font_scores*size_s)*0.6), padding_y + (size_info_bar - padding_y) * 0.35);

    }
    void setName(std::string name)
    {
        name.push_back(':');
        Name.setString(name);
    }
    void setTime(int tm)
    {
        tm = time_out - tm;
        int db = log10(tm) - size_t;
        std::string temp = "", s = std::to_string(tm);
        for (int i = 0; i < db; i++) { temp.push_back(' '); }
        Timer.setString(temp + s);
    }
    void setScore(int sc)
    {
        int db = log10(sc) - size_t;
        std::string temp = "", s = std::to_string(sc);
        for (int i = 0; i < db; i++) { temp.push_back('0'); }
        Score.setString(temp + s);

    }
    void draw(RenderWindow& app)
    {
        app.draw(rec_bg);
        app.draw(Timer);
        app.draw(Name);
        app.draw(Score);
    }
};

class Game
{
private:
    std::vector<BootlType> types_bootl;
    std::vector<Bootl> list_bootls;
    std::vector<int> counts_bootls;
    int MaxBootles;
    
    std::vector<EnemyType> types_enemy;
    std::vector<Enemy> list_enemys;
    std::vector<int> counts_enemys;
    int MaxEnemys;

    Background bg;
    std::vector <char> map;

    InfoBar info;

    int previos_time, time;
    Player player;
    std::string name;
public:
    Game(){}
    Game(std::string floor_file, std::string wall_file, std::string skin_file, 
        std::vector<std::string> list_bootls_files, std::vector<int> list_bootls_points, 
        std::vector<std::string> list_enemys_files, std::vector<int> list_enemys_points)
    {
        generateMap();
        bg.LoadMap(map);
        bg.LoadTextures(floor_file, wall_file);
        bg.RefreshTextureMap();
        MaxBootles = bg.getFreeCells() * relation_cels_bootls;
        player.set();
        player.InitFromFiles(skin_file);
        player.updateCoords();
        name = "Player";
        for (int i = 0; i < list_bootls_files.size(); i++)  
        {
            types_bootl.push_back(BootlType());
            types_bootl[i].InitFromFile(list_bootls_files[i], list_bootls_points[i], i);
            counts_bootls.push_back(0);
        }
        UpdateBootls();
        MaxEnemys = bg.getFreeCells() * relation_cels_enemys;
        for (int i = 0; i < list_enemys_files.size(); i++)
        {
            types_enemy.push_back(EnemyType());
            types_enemy[i].InitFromFile(list_enemys_files[i], list_enemys_points[i], i);
            counts_enemys.push_back(0);
        }
        UpdateEnemys();
        previos_time = clock()/1000;
        time = 0;
        info.Load();
        info.setName(name);
        info.setScore(0);
        info.setTime(time);

    }
    void UpdateBootls()
    {
        for (;list_bootls.size() < MaxBootles;)
        {

            addRandomBootl();
        }
    }
    void UpdateEnemys()
    {
        for (; list_enemys.size() < MaxEnemys;)
        {

            addRaandomEnemy();
        }
    }
    void addRandomBootl()
    {
        int rand_index_type = rand() % (types_bootl.size());
        list_bootls.push_back(Bootl(types_bootl[rand_index_type]));
        float r = list_bootls[list_bootls.size() - 1].getRadius();
        PointInt p_blocs = bg.getRandonmFreeBloc(player.getPos(), player.getRadius(), r, list_bootls, list_bootls.size() - 1);
        float _x, _y, range = bloc_size - 2*r - 4;
        _x = rand() % int(range) + (p_blocs.x) * bloc_size + r + 2;
        _y = rand() % int(range) + (p_blocs.y) * bloc_size + r + 2;
        list_bootls[list_bootls.size() - 1].setCoordBloc(p_blocs.x, p_blocs.y);
        list_bootls[list_bootls.size() - 1].setCoord(_x, _y);
        list_bootls[list_bootls.size() - 1].updateCoords();
    }
    void addRaandomEnemy()
    {
        int rand_index_type = rand() % (types_enemy.size());
        list_enemys.push_back(Enemy(types_enemy[rand_index_type], player.getScore()));
        float r = list_enemys[list_enemys.size() - 1].getRadius();
        std::vector <PointInt> way = bg.getWayEnemy(player.getPos(), player.getRadius(), r, list_enemys);
        float _x, _y;
        _x = (way[0].x) * bloc_size + bloc_size / 2;
        _y = (way[0].y) * bloc_size + bloc_size / 2;
        list_enemys[list_enemys.size() - 1].setCoord(_x, _y);
        list_enemys[list_enemys.size() - 1].updateCoords();
        if (way.size() == 1)
        {
            way = {};
        }
        list_enemys[list_enemys.size() - 1].setWay(way);
        list_enemys[list_enemys.size() - 1].updateDirection();

    }
    void ColisionWithBootl()
    {
        for (int i = 0; i < list_bootls.size(); i++)\
        {
            float d = list_bootls[i].getRadius() + player.getRadius() - distanse(list_bootls[i].getPos(), player.getPos());
            if (d >= 0)
            {
                player.addScore(list_bootls[i].getScore());
                counts_bootls[list_bootls[i].getType()]++;
                list_bootls.erase(list_bootls.begin() + i);
                UpdateBootls();
                break;
            }
        }
    }
    bool ColisionWithEnemy()
    {
        for (int i = 0; i < list_enemys.size(); i++)\
        {
            float d = list_enemys[i].getRadius() + player.getRadius() - distanse(list_enemys[i].getPos(), player.getPos());
            if (d >= 0)
            {
                float plus;
                std::cout << player.getScore() << " " << list_enemys[i].getScore() << std::endl;
                if (player.getScore() < list_enemys[i].getScore())
                {
                    plus = -1 * list_enemys[i].getScore() * relation_collision_die;
                    if (!player.addScore(plus)) { return false; }
                    PointInt new_bloc = bg.getRandonmFreeBloc(list_enemys, list_enemys.size());
                    player.setCoord(new_bloc.x * bloc_size + bloc_size / 2, new_bloc.y * bloc_size + bloc_size / 2);
                    player.updateCoords();
                }
                else
                {
                    plus = list_enemys[i].getScore() * relation_collision_kill;
                    player.addScore(plus);
                    counts_enemys[list_enemys[i].getType()]++;
                    list_enemys.erase(list_enemys.begin() + i);
                    UpdateEnemys();
                }
                break;
            }
        }
        return true;
    }
    void MovePlayerUp()
    {
        player.Move(0, -1 * step, bg);
    }
    void MovePlayerDown()
    {
        player.Move(0, step, bg);
    }
    void MovePlayerRight()
    {
        player.Move(step, 0, bg);
    }
    void MovePlayerLeft()
    {
        player.Move(-1 * step, 0, bg);
    }
    bool update()
    {
        ColisionWithBootl();
        if (!ColisionWithEnemy()) { return false; }
        int next_time = clock() / 1000;
        bool check = false;
        if (next_time != previos_time)
        {
            previos_time = next_time;
            time += 1;
            info.setTime(time);
            if (time == time_out)
            {
                return false;
            }
            player.update();
            check = true;
        }
        info.setScore(player.getScore());
        for (int i = 0; i < list_enemys.size(); i++) 
        {
            bool check_update_enemy = list_enemys[i].checkUpdate();

            if (check_update_enemy && bg.CountMoved(list_enemys[i].getPos(), list_enemys[i].getRadius(), list_enemys[i].getPointD()))
            {
                list_enemys[i].update();
            }
            list_enemys[i].updateDirection();
        }
        for (int i = 0; i < list_bootls.size(); i++)\
        {
            if (check && !list_bootls[i].update())
            {
                player.addScore(list_bootls[i].getScore());
                list_bootls.erase(list_bootls.begin() + i);
                UpdateBootls();
                continue;
            }
            list_bootls[i].update_milisecond();
        }
        return true;

    }
    void draw(RenderWindow& app)
    {
        info.draw(app);
        bg.draw(app);
        player.draw(app);
        for (int i = 0; i < list_bootls.size(); i++)\
        {
            list_bootls[i].draw(app);
        }
        for (int i = 0; i < list_enemys.size(); i++)\
        {
            list_enemys[i].draw(app);
        }
    }
    // Генерація лабіринтів
    void ShowMapConsole()
    {
        for (int y = 0; y < size_matrix_h; y++)
        {
            for (int x = 0; x < size_matrix_w; x++)
            {
                std::cout << map[y * size_matrix_w + x] << " ";
            }
            std::cout << std::endl;
        }
    }
    void DFS_generator(int _x, int _y)
    {
        std::vector<std::vector<int>> dir{ {1,0},{-1,0},{0,1},{0,-1} };
        std::random_shuffle(dir.begin(), dir.end());
        setValueMap(_x, _y, '-');
        for (int i = 0; i < 4; ++i)
        {
            if (_x + 2 * dir[i][0] >= 0 && _x + 2 * dir[i][0] <= size_matrix_w - 1 && _y + 2 * dir[i][1] >= 0 && _y + 2 * dir[i][1] <= size_matrix_h - 1
                && getValueMap(_x + 2 * dir[i][0], _y + 2 * dir[i][1]) == '#')
            {
                setValueMap(_x + dir[i][0], _y + dir[i][1], '-');
                DFS_generator(_x + 2 * dir[i][0], _y + 2 * dir[i][1]);
            }
        }
    }
    void generateMap()
    {
        map.clear();
        fill_map();
        PointInt p((rand() % (size_matrix_w - 2) + 1), (rand() % (size_matrix_h - 2) + 1));
        setValueMap(0, 0, '-');
        setValueMap(size_matrix_w - 1, size_matrix_h - 1, '-');
        DFS_generator((rand() % (size_matrix_w - 2) + 1), (rand() % (size_matrix_h - 2) + 1));
        setValueMap(1, 0, '-');
        setValueMap(0, 1, '-');
        setValueMap(size_matrix_w - 2, size_matrix_h - 1, '-');
        setValueMap(size_matrix_w - 1, size_matrix_h - 2, '-');
        
    }
    void fill_map()
    {
        map.clear();
        for (int i = 0; i < size_matrix_h * size_matrix_w; i++)
        {
            map.push_back('#');
        }
    }
    void setValueMap(int x, int y, char value)
    {
        map[y * size_matrix_w + x] = value;
    }
    char getValueMap(int x, int y)
    {
        return map[y * size_matrix_w + x];
    }


};


class Finish
{
private:
    std::vector<Bootl> btl_types;
    std::vector<Enemy> enn_types;
    std::vector<Text> enn_count;
    std::vector<Text> btl_count;
    Text Score;
    RectangleShape rr;
public:
    Finish(std::vector<Bootl> _btl_type, std::vector<Enemy> _enm_type, std::vector<int> _enn_count, std::vector<int> _btl_count, int score)
    {
        rr.setFillColor(Color(255, 255, 255, 128));
        rr.setSize(Vector2f(W, H + size_info_bar));
        rr.setPosition(padding_x, padding_y);
        btl_types = _btl_type;
        for (int i = 0; i < btl_types.size(); i++)
        {

        }
    }
};


int main()
{
    RenderWindow app(sf::VideoMode(W + 2*padding_x, H + 2*padding_y + size_info_bar), "A stranger in a domintory");
    app.setVerticalSyncEnabled(true);
    Image icon;
    icon.loadFromFile("images\\icon.png");
    app.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    std::string floor = "images\\floor.png", wall = "images\\wall.png", skin{ "images\\Taras.png" };
    std::vector<std::string> alko_f{ std::string("images\\bear.png"), std::string("images\\wine.png"), std::string("images\\vodka.png") },
        vahta_f = { std::string("images\\vahter.png")};
    std::vector<int> alko_k{ 5, 10, 15 }, vahta_k{ 15 };
    Game Controller(floor, wall, skin, alko_f, alko_k, vahta_f, vahta_k);

    std::string status = "game";

    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();
        }
        if (status == "game")
        {
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                Controller.MovePlayerRight();
            }
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                Controller.MovePlayerLeft();
            }
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                Controller.MovePlayerDown();
            }
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                Controller.MovePlayerUp();
            }
        }

        app.clear();
        if (status == "game")
        {
            if (!Controller.update())
            {
                status = "finish";
            }
            
        }
        Controller.draw(app);
        app.display();
    }

    return 0;
};
