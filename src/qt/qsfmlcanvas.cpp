#include "qt/qsfmlcanvas.hpp"

void QSFMLCanvas::OnInit() {

    std::cout << "oninit" << std::endl;
    // sf::Texture texture;
    // // Load the image
    // texture.loadFromFile("../static/gamefon.jpg");

    // // Setup the sprite
    // mySprite.setTexture(texture);
    // mySprite.setPosition(0, 0);

    clear(sf::Color(0, 128, 0));
    display();
}

void QSFMLCanvas::OnUpdate() {
    GameEvent event{EventType::FINISH};
    if (pollEvent(event)) game->HandleInput(event);
    game->Render();
}

QPaintEngine* QSFMLCanvas::paintEngine() const { return 0; }

void QSFMLCanvas::showEvent(QShowEvent*) {
    
    if (!myInitialized) {
        OnInit();

        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }
}

void QSFMLCanvas::paintEvent(QPaintEvent*) {
    OnUpdate();
    display();
}

void QSFMLCanvas::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() == Qt::LeftButton) {
        GameEvent ge{EventType::CHOSE};
        ge.cords = sf::Vector2u{e->localPos().x() / 63, e->localPos().y() / 63};
        pushEvent(ge);
    }
}

void QSFMLCanvas::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Backspace) {
        GameEvent ge{EventType::UNCHOSE};
        pushEvent(ge);
    }

    if (e->key() == Qt::Key_1) {
        GameEvent ge{EventType::CREATE_OBJECT};
        ge.unit_type = B;
        pushEvent(ge);
    }

    if (e->key() == Qt::Key_Return) {
        GameEvent ge{EventType::FINISH};
        pushEvent(ge);
    }

    if (e->key() == Qt::Key_Space) {
        GameEvent ge{EventType::FINISH};
        ge.cmds = "c b 2 3 c b 2 4 c b 2 5 e";
        pushEvent(ge);
    }

    if (e->key() == Qt::Key_Shift) {
        GameEvent ge{EventType::FINISH};
        ge.cmds = "a 2 4 3 4 a 2 4 3 4 a 2 4 3 4 a 2 4 3 4 a 2 4 3 4 e";
        pushEvent(ge);
    }
}

void QSFMLCanvas::pushEvent(GameEvent& ev) { GameEvents.push_back(ev); }

QSFMLCanvas::QSFMLCanvas(QWidget* Parent, const QPoint& Position,
                         const QSize& Size, unsigned int FrameTime)
    : QWidget(Parent), myInitialized(false), game(nullptr) {
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setFocusPolicy(Qt::StrongFocus);

    move(Position);
    resize(Size);

    myTimer.setInterval(FrameTime);

    parentwidget = Parent;
}

bool QSFMLCanvas::pollEvent(GameEvent& ev) {
    if (GameEvents.size() == 0) return false;

    ev = GameEvents.back();
    GameEvents.pop_back();

    return true;
}

QSFMLCanvas::~QSFMLCanvas() {
    if (game != nullptr) delete game;
}
