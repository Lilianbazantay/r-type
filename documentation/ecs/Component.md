# Component :

## Contexte :
In our code, every `components` are part of entities. To allow us to Create more different entities, they are stored into a list of [IComponent](../../src/ecs/main.cpp).
The advantages :
- more modular
- easier to replace and modify there behaviour
- easier to understand

## [IComponent](../../src/ecs/IComponent.hpp) :
[IComponent](../../src/ecs/IComponent.hpp) is the interface of every `components`, it gives them a base pattern to respect.
IComponent:
```
class IComponent
{
    protected:
        ComponentType _type = ComponentType::UNKNOWN;

    public:
        virtual ~IComponent() = default;

        // SET/GET
        virtual ComponentType GetType() const { return _type; } // Return the type of Component
        virtual void SetType(ComponentType type) { _type = type; } // Set the type of Component
};
```
Each component has a type to simplify the recognition of each one. Those type are into an enum `ComponentType`, this enum is stored into the file [IComponent.hpp](../../src/ecs/IComponent.hpp).
```
enum class ComponentType {
    HP,
    POSITION,
    VELOCITY,
    DIRECTION,
    ENTITY_SPAWNER,
    SPRITE,
    ANIMATED_SPRITE,
    SOUND,
    COOLDOWN,
    CLOCK,
    HITBOX,
    ATTACK,
    GRAVITY,
    STRATEGY,
    UNKNOWN
};
```

## Component Summary :
- [Animated Sprite](#animated-sprite)
- [Attack](#attack)
- [Clock](#clock)
- [Cooldown](#cooldown)
- [Direction](#direction)
- [Entity Spawner](#entity-spawner)
- [Gravity](#gravity)
- [Hitbox](#hitbox)
- [Hp](#hp)
- [Position](#position)
- [Sound](#sound)
- [Sprite](#sprite)
- [Strategy](#strategy)
- [Velocity](#velocity)

## Component explanation

### Animated Sprite
[link to the file](../../src/ecs/Component/AnimatedSprite.hpp)

This component manages an `Animated Sprite`, it is handled with the sfml.
```
class AnimatedSprite : public IComponent
{
private:
    sf::Texture _texture;
    sf::Sprite _sprite;
    sf::IntRect _currentRect;
    int _number_of_sprite_x;
    int _number_of_sprite_y;
    float _animation_rate;
    float _elapsed_time = 0.f;
    int _current_frame_x = 0;
    int _current_frame_y = 0;
    bool _is_visible = true;

public:
    // ANIMATED SPRITE
    AnimatedSprite(const std::string& file_path, float size_x, float size_y, int number_of_sprite_x, int number_of_sprite_y);
    ~AnimatedSprite() override = default;

    // GET / SET
    sf::Sprite& GetSprite();
    void SetTexture(const std::string& file_path);

    // SIZE
    std::pair<float, float> GetSize() const;
    void SetSize(std::pair<float, float> size);
    void SetSize(float size_x, float size_y);
    void multiplySize(std::pair<float, float> multiplicator);
    void multiplySize(float multiplicator_x, float multiplicator_y);
    void deviseSize(std::pair<float, float> divisor);
    void deviseSize(float divisor_x, float divisor_y);

    // NUMBER OF SPRITE
    std::pair<int, int> GetNumberOfSprite() const;
    void SetNumberOfSprite(std::pair<int, int> number_of_sprite);
    void SetNumberOfSprite(int number_of_sprite_x, int number_of_sprite_y);

    // ANIMATION
    float GetAnimationRate() const;
    void SetAnimationRate(float animation_rate);

    // VISIBILITY
    bool GetVisibility() const;
    void SetVisibility(bool visibility);
    void show();
    void hide();

    // UPDATE
    void update(float deltaTime);
};
```
##### ANIMATED SPRITE Part
Contains a custom constructor, it sets the `filepath`, the `width`, the `length`, the `number of frame in the width` and the `number of frame in the length`.

##### GET / SET Part
Set and get the sprite to secure the modification of the data.

##### SIZE Part
- `GetSize` return the size as a float
- `SetSize` set the size with either a pair of float either two float
- `multiplySize` multiply the size of the sprite with either a pair of float or two float
- `divideSize` divide the size of the sprite with either a pair of float or two float if one one the two divider in equal to 0, do nothing

##### NUMBER OF SPRITE Part
- `GetNumberOfSprite` return the number of frame as a pair of int the first one in width and the second one in length
- `SetNumberOfSprite` set the number of sprite with either a pair of int or two int

##### ANIMATION Part
- `GetAnimationRate` return the animation rate as a float
- `SetAnimationRate` set the animation rate with a float

##### VISIBILITY Part
- `GetVisibility` return if the sprite is visible or not with a bool
- `SetVisibility` set if the sprite is visible or not with a bool
- `show` set the sprite visibility to true
- `hide` set the sprite visibility to false

##### UPDATE Part
- `update` update the frame with a float that advance the animation to the good frame

### Attack
[link to the file](../../src/ecs/Component/Attack.hpp)

This component contains the statistics of an entity.
```
class Attack : public IComponent
{
    private:
        float _damage;
        float _fireRate;
        Timestamp lastShot;
        float _remaining;
    public:
        // ATTACK
        Attack(float damage, float firerate) : _damage(damage), _fireRate(firerate) {
            _type = ComponentType::ATTACK;
        };
        ~Attack() override = default;

        bool fire();

        // SET/GET
        std::pair<float, float> GetAttack();
        void SetAttack(int damage, float firerate);

        // ADD/SUB
        void AddDamage(float damage);
        void SubDamage(float damage);

        void AddFireRate(float firerate);
        void SubFireRate(float firerate);
};
```
##### ATTACK Part
- `Attack` is the constructor that set the damage and the firerate
- `~Attack` is a default destructor for the class
- `fire` check if you can shoot and return true is you can and false if you can't

##### SET / GET Part
- `GetAttack` return the attack damage and firerate with a pair of float
- `SetAttack` set the attack damage and firerate with two float

##### ADD / SUB Part
- `AddDamage` add a float taken as parameter to the damage
- `SubDamage` sub a float taken as parameter to the damage
- `AddFirerate` add a float taken as parameter to the firerate
- `SubFirerate` sub a float taken as parameter to the firerate

### Clock
[link to the file](../../src/ecs/Component/Clock.hpp)

This component contains a `clock` and wrap those from the sfml.
```
class Clock : public IComponent
{
private:
    sf::Clock _clock;

public:
    Clock() = default;
    ~Clock() override = default;

    sf::Time GetElapseTime();
    void RestartClock();
};
```
- `Clock` is a default constructor for the class
- `~Clock` is a default destructor for the class
- `GetElapseTime` return the time elapse since begining of the game
- `RestartClock` reset the clock to 0;

### Cooldown
[link to the file](../../src/ecs/Component/Cooldown.hpp)

This component contains a `cooldown`.
```
class Cooldown : public IComponent
{
private:
    double _length;
    double _remaning = -1;
    double _timestamp_activation = -1;

    double GetTimeSeconds();

public:
    Cooldown(double length);
    ~Cooldown() override = default;

    double GetLength();
    void SetLength(double new_length);

    void LaunchCooldown();
    bool CheckCooldown();
};
```
- `Cooldown` is the constructor that set the length of the of the cooldown
- `~Cooldown` is a default constructor
- `GetLength` return the length of the cooldown as a float
- `SetLength` set the length of the cooldown as a float
- `LaunchCooldown` start the cooldown
- `CheckCooldown` return if the cooldown is over with a bool. If it's true, the cooldown is over else it return false

### Direction
[link to the file](../../src/ecs/Component/Direction.hpp)

This component set the direction of the entity with a vector normal or an angle
```
class Direction : public IComponent
{
private:
    float x;
    float y;
    float angle;

    void updateVectorFromAngle();
    void updateAngleFromVector();

public:
    // Constructors
    Direction(float x = 0.f, float y = 0.f);
    Direction(float angleDeg);

    ~Direction() override = default;

    // GET / SET VECTOR
    std::pair<float, float> GetDirection() const;
    void SetDirection(float newX, float newY);
    void SetDirection(const std::pair<float, float>& vec);

    // GET / SET ANGLE
    float getAngle() const;
    void setAngleDegrees(float deg);

    // NORMALIZE VECTOR
    void normalize();

    // ROTATION (degrees)
    void rotateDegrees(float angleDeg);
};
```
##### CONSTRUCTION Part
- `Construction` is the constructor the set the vector normal and the angle
- `~Construction` is the default constructor

##### GET / SET Angle Part
- `GetDirection` return the direction as a pair of float
- `SetDirection` set the direction with two float or a pair of float

##### GET / SET VECTOR Part
- `getAngle` return the angle of the direction as a float
- `setAngleDegrees` set the angle of the direction with a float

##### NORMALIZE VECTOR Part
- `normalize` make the vector as an unitary vector

##### ROTATION Part
- `rotationDegrees` add the angle given as parameter to the angle of the direction

### Entity Spawners
[link to the file](../../src/ecs/Component/EntitySpawner.hpp)

This component allow the game engine to make entities as bullet, enemy or whatever you want to make spawn
```
class EntitySpawner : public IComponent
{
private:
    Cooldown _cooldown;
    IMediatorEntity *_entity;
    int _nb_of_spawn;
    bool _is_activated;

public:
    // CONSTRUCTOR
    EntitySpawner(double cooldown_length, IMediatorEntity *entity, int number_of_spawn, int is_activated);
    ~EntitySpawner() override = default;

    // GET / SET
    Cooldown GetCooldown();
    void SetCooldown(Cooldown new_cooldown);
    double GetCooldownLength();
    void SetCooldownLength(double new_cooldown);

    IMediatorEntity *GetEntity();
    void SetEntity(IMediatorEntity *new_entity);

    int GetNumberOfSpawn();
    void SetNumberOfSpawn(int new_number_of_spawn);

    // ACTIVATION
    bool IsActivated();
    void Enable();
    void Disable();

    // SPAWN
    void Spawn();
    bool CanSpawn();
};
```
##### CONSTRUCTOR Part
- `EntitySpawner` is the constructor that set the length of the cooldown between every spawn, the Entity to spawn, the number of time the entity can spawn and if the spawner is activated
- `~EntitySpawner` is a default destructor of the class

##### GET / SET Part
- `GetCooldown` return the component cooldown
- `SetCooldown` set the component cooldown
- `GetCooldownLength` return the cooldown length as a float
- `SetCooldownLength` set the cooldown length with a float
- `GetEntity` return the entity that will spawn
- `SetEntity` set the entity that will spawn
- `GetNumberOfSpawn` return the number of entity that will spawn
- `SetNumberOfSpawn` set the number of entity that will spawn

##### ACTIVATION Part
- `IsActivated` return if the spawn is activated
- `Enable` activate the spawn of entity
- `Disable` unactivate the spawn of entity

##### SPAWN Part
- `Spawn` React to a spawn of entity by launching the cooldown and sub 1 to the number of spawn
- `CanSpawn` return if the spawner can spawn if it's true, the cooldown is over and the number of entity you the spawner can spawn is more than 0, else it return false

### Gravity
[link to the file](../../src/ecs/Component/Gravity.hpp)

This component contains the gravity stat of an entity.
```
class Gravity: public IComponent
{
private:
    bool _is_activated;
    float _attraction_strength;

public:
    // CONSTRUCTOR
    Gravity(bool is_activated, float attraction_strength);
    ~Gravity() override = default;

    // GET / SET
    bool IsActivated();
    void SetActivationState(bool new_state);
    void Enable();
    void Disable();

    float GetAttractionStrength();
    void SetAttractionStrength(float new_strength);

    // STRENGTH
    void MultiplyAttractionStrength(float multiplier);
    void DivideAttractionStrength(float divider);
};
```
##### CONSTRUCTION Part
- `Gravity` set if the gravity is activated for the entity and set the strength of the gravity
- `~Gravity` is a default destructor

##### SET / GET Part
- `IsActivated` return bool, it return true if the gravity is activated, else it return false
- `SetActivationState` set if the gravity is activated or not with a bool
- `Enable` activate the gravity
- `Disable` unactivate the gravity
- `GetAttractionStrength` return the attraction strength as a float
- `SetAttractionStrength` set the attraction with a float


##### STRENGTH Part
- `MultiplyAttractionStrength` multiply the attraction strength with a float in parameter
- `DivideAttractionStrength` divide the attraction strength with a float in parameter. If it's equal to 0, do nothing

### Hitbox
[link to the file](../../src/ecs/Component/HitBox.hpp)

This Component allow the ecs to check collisions between entities with masks and layers
```
class Hitbox : public IComponent
{
    private:
        float _sizeX;
        float _sizeY;
        std::vector<int> _layers;
        std::vector<int> _masks;
        int _damage;

    public:
        // Constructor
        Hitbox(float sizeX, float sizeY, int damage);
        Hitbox(float sizeX, float sizeY, int damage, std::vector<int> layers, std::vector<int> mask);
        ~Hitbox() override = default;

        // Size
        std::pair<float, float> GetHitboxSize();
        void SetHitboxSize(float sizeX, float sizeY);
        void SetHitboxSize(std::pair<float, float> size);

        // Layers
        void AddLayer(int layer);
        void SubLayer(int layer);
        std::vector<int> GetLayers();
        void SetLayers(std::vector<int> layers);

        // Masks
        void AddMask(int mask);
        void SubMask(int mask);
        std::vector<int> GetMask();
        void SetMask(std::vector<int> mask);

        // Damage
        int GetDamage();
        void SetDamage(int new_damage);
        void MultiplyDamage(int multiplyer);
        void DivideDamage(int divider);
};
```
##### CONSTRUCTOR Part
- `Hitbox` is the constructor that set the size and damage and optionally the layer and mask
- `~Hitbox` is a default constructor

##### SIZE Part
- `GetHitboxSize` return the hitbox size as a pair of float
- `SetHitboxSize` set the hitbox size with a pair of float or two float

##### LAYERS Part
- `AddLayer` add a layer to the layer list
- `SubLayer` sub a layer to the layer list
- `GetLayers` return the list of layer the hitbox can interact as a list of int
- `SetLayers` set the list of layer the hitbox can interact with a list of int

##### MASKS Part
- `AddMask` add a mask to the mask list
- `SubMask` sub a mask to the mask list
- `GetMask` return the list of mask the hitbox can interact as a list of int
- `SetMask` set the list of mask the hitbox can interact with a list of int

##### DAMAGE Part
- `GetDamage` return the damage as an int done by the collision
- `SetDamage` set the damage with an int done by the collision
- `MultiplyDamage` multiply the damage with an int
- `DivideDamage` divide the damage with an int. if the divider equal 0 do nothing

### Hp
[link to the file](../../src/ecs/Component/Hp.hpp)

This component manages the life of an entity
```
class Hp : public IComponent
{
    private:
        int _value;
    public:
        // CONSTRUCTOR
        Hp(int value) : _value(value) {
            _type = ComponentType::HP;
        };
        ~Hp() override = default;

        // SET/GET
        int GetHp();
        void SetHp(int value);

        // ADD/SUB
        void AddHp();
        void AddHp(int value);
        void SubHp();
        void SubHp(int value);
};
```
##### CONSTRUCTOR Part
- `HP` is the constructor, it sets the hp of an entity
- `~HP` is a default constructor

##### SET / GET Part
- `GetHp` return the hp as an int
- `SetHp` set the hp with an int

##### ADD / SUB Part
- `AddHp` add hp with an int or add one hp if no parameter
- `SubHp` sub hp with an int or sub one hp if no parameter

### Position
[link to the file](../../src/ecs/Component/Position.hpp)

This component manages the position of an entity
```
class Position : public IComponent
{
    private:
        float _x;
        float _y;
    public:
        // CONSTRUCTOR
        Position(float x = 0, float y = 0) : _x(x), _y(y)
        {
            _type = ComponentType::POSITION;
        };
        ~Position() = default;

        // SET/GET
        std::pair<float, float> GetPosition();
        void SetPosition(float newX, float newY);
        void SetPosition(std::pair<float, float> newPos);

        // MOVEMENT
        void moveUp();
        void moveUp(float distance);

        void moveDown();
        void moveDown(float distance);

        void moveLeft();
        void moveLeft(float distance);

        void moveRight();
        void moveRight(float distance);
};
```
##### CONSTRUCTOR Part
- `Position` is the constructor that set the position in x and in y
- `~Position` is a default constructor

##### SET / GET Part
- `GetPosition` return the position as a pair of float
- `SetPosition` set the position with a pair of float or two float

##### MOVEMENT Part
- `moveUp` move the position to the top by one without parameter or with a float as parameter
- `moveDown` move the position to the bottom by one without parameter or with a float as parameter
- `moveLeft` move the position to the left by one without parameter or with a float as parameter
- `moveRight` move the position to the right by one without parameter or with a float as parameter

### Sound
[link to the file](../../src/ecs/Component/Sound.hpp)

This component allow the entity component system to add sound to the game after an event or in the background, its manages with sfml
```
class Sound : public IComponent
{
private:
    sf::SoundBuffer _buffer;
    sf::Sound _sound;
    float _volume;
    bool _is_looping = false;

public:
    // CONSTRUCTOR
    Sound(const std::string& file_path, float volume, bool is_looping);
    ~Sound() override = default;

    // SOUND
    sf::Sound& GetSound();
    void SetBuffer(const std::string& file_path);

    // VOLUME
    float GetVolume() const;
    void SetVolume(float volume);

    // LOOP
    bool GetIsLooping() const;
    void SetIsLooping(bool is_looping);
    void Loop();
    void DontLoop();

    // PLAY / STOP
    void Play();
    void Stop();
};
```
##### CONSTRUCTOR Part
- `Sound` is the constructor for the class, it sets the filepath to the sound, the volume and if the sound is looping or not
- `~Sound` is a default constructor

##### SOUND Part
- `GetSound` return the sound as a sf::Sound
- `SetBuffer` set the sound with a std::string that is a filepath to the sound

##### VOLUME Part
- `GetVolume` return the volume of the sound as a float
- `SetVolume` set the volume of the sound with a float

##### LOOP Part
- `GetIsLooping` return if the sound is looping or not as a bool
- `SetIsLooping` set if the sound is looping or not with a bool
- `Loop` make the sound loop
- `DontLoop` make the sound stop looping

##### PLAY / STOP Part
- `Play` make the sound play
- `Stop` make the sound stop

### Sprite
[link to the file](../../src/ecs/Component/Sprite.hpp)

This component manages a `Sprite`, it is handled with the sfml.
```
class Sprite : public IComponent
{
private:
    sf::Texture _texture;
    sf::Sprite _sprite;
    bool _is_visible = true;

public:
    Sprite(const std::string& file_path, float size_x, float size_y);
    ~Sprite() override = default;

    // SET / GET
    sf::Sprite GetSprite();
    void SetTexture(const std::string& file_path);

    // SIZE
    std::pair<float, float> GetSize() const;
    void SetSize(std::pair<float, float> size);
    void SetSize(float size_x, float size_y);

    void multiplySize(std::pair<float, float> multiplicator);
    void multiplySize(float multiplicatorX, float multiplicatorY);
    void deviseSize(std::pair<float, float> divisor);
    void deviseSize(float divisor_x, float divisor_y);

    // Visibility
    bool GetVisibility() const;
    void SetVisibility(bool visibility);
    void show();
    void hide();
}
```
##### SPRITE Part
- `Sprite` is a custom constructor, it sets the `filepath`, the `width`, the `length`.
- `~Sprite` is a default constructor

##### GET / SET Part
Set and get the sprite to secure the modification of the data.

##### SIZE Part
- `GetSize` return the size as a float
- `SetSize` set the size with either a pair of float either two float
- `multiplySize` multiply the size of the sprite with either a pair of float or two float
- `divideSize` divide the size of the sprite with either a pair of float or two float if one of the two divider in equal to 0, do nothing

##### VISIBILITY Part
- `GetVisibility` return if the sprite is visible or not with a bool
- `SetVisibility` set if the sprite is visible or not with a bool
- `show` set the sprite visibility to true
- `hide` set the sprite visibility to false

### Strategy
[link to the file](../../src/ecs/Component/Strategy.hpp)

This is a component that contains the strategy pattern of en entity like a enemy or a boss
```
class Strategy : public IComponent
{
private:
    std::vector<std::pair<StrategyType, int>> _pattern;

public:
    // CONSTRUCTOR
    Strategy(std::vector<std::pair<StrategyType, int>> strategy);
    ~Strategy() override = default;

    // GET / SET
    std::vector<std::pair<StrategyType, int>> GetPattern();
    void SetPattern(std::vector<std::pair<StrategyType, int>> new_pattern);

    // MODIFY PATTERN
    void AddElementToPattern(std::pair<StrategyType, int> element);
    void AddElementToPattern(StrategyType action, int value);

    std::vector<std::pair<StrategyType, int>> CreateRandomPattern(int number_of_element, int max_element_value, bool is_infinitely_looping);

    void RemoveElementToPattern(size_t index);
};
```

This enum contains every action available for an entity
```
#define NUMBER_STRATEGY_POSSIBILITY 6

enum StrategyType {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    SHOOT,
    LOOP,
};
```
##### CONSTRUCTOR Part
- `Strategy` is the constructor, it sets the pattern as a vector of pair that contains a StrategyType and a value
- `~Strategy` is the default destructor

##### SET / GET Part
- `GetPattern` return the pattern as a vector of pair that contains a StrategyType and it value
- `SetPattern` set the pattern with a vector of pair that contains a StrategyType and it value

##### MODIFY PATTERN Part
- `AddElementToPattern` add en element to the pattern with a pair that contains a StrategyType and a int or without a pair and just a strategyType and an int
- `CreateRandomPattern` it will generate a random pattern, it take in parameter the number of element in the pattern, the max of each element and is the pattern need to be a infinite loop or not
- `RemoveElementToPattern` remove an element of the pattern with a index that is a size_t

### Velocity
[link to the file](../../src/ecs/Component/Velocity.hpp)

This Component contains the velocity of a component as a enemy, a bullet or a player
```
class Velocity : public IComponent
{
    private:
        float _value;
    public:
        // CONSTRUCTOR
        Velocity(float value) : _value(value) {
            _type = ComponentType::VELOCITY;
        };
        ~Velocity() override = default;

        // SET/GET
        float GetVelocity();
        void SetVelocity(float value);

        // ADD/SUB
        void AddVelocity(float value);
        void SubVelocity(float value);
};
```
##### CONSTRUCTOR Part
- `Velocity` is the constructor, that set the value of the velocity
- `~Velocity` is a default destructor

##### SET / GET Part
- `GetVelocity` return the velocity as a float
- `SetVelocity` set the velocity with a float


##### ADD / SUB Part
- `AddVelocity` add to the velocity, the float given as parameter
- `SubVelocity` sub to the velocity, the float given as parameter
