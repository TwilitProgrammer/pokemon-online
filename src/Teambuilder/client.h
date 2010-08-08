#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui>
#include "analyze.h"
#include "../PokemonInfo/networkstructs.h"
#include "../PokemonInfo/battlestructs.h"
#include "centralwidget.h"

class MainEngine;
class BaseChallengeWindow;
class QIdTreeWidgetItem;
class BattleWindow;
class BaseBattleWindow;
class QScrollDownTextEdit;
class PMWindow;
class ControlPanel;
class RankingDialog;
class BattleFinder;
class FindBattleData;
class Channel;

/* The class for going online.

    It displays the mainchat, the players list, ... and also have the dialog engine in it*/

class Client : public QWidget, public CentralWidgetInterface
{
    Q_OBJECT
public:
    Client(TrainerTeam *, const QString &url, const quint16 port);

    TrainerTeam *team();
    QMenuBar *createMenuBar(MainEngine *w);

    void printLine(const QString &line);
    void printHtml(const QString &html);
    /* Prints a line to all the channels which have that player */
    void printLine(int playerid, const QString &line);
    void cancelFindBattle(bool verbose=true);
    bool playerExist(int id) const;
    QString name(int id) const;
    QString ownName() const;
    bool battling() const;
    bool busy() const;
    bool hasChannel(int channelid) const;
    bool away() const;
    int id(const QString &name) const;
    int currentChannel() const;
    Channel *channel(int channelid);
    int ownId() const ;
    int ownAuth() const ;
    int auth(int id) const ;
    bool isIgnored(int id) const;
    QString authedNick(int id) const;
    QColor color(int id) const;
    QString tier(int player) const;
    void changeName(int player, const QString &name);
    /* Resets fade away counter */
    void refreshPlayer(int id);

    enum Status {
        Available = 0,
        Away,
        Battling,
        Ignored,
        LastStatus
    };
    QIcon statusIcon(int auth, Status status) const;

    void seeChallenge(const ChallengeInfo &c);

    PlayerInfo player(int id) const;
    BasicInfo info(int id) const;

    void removePlayer(int id);
    void removeBattleWindow(int id);

    QList<QColor> chatColors;
    QList<QIcon> statusIcons;
    QIcon chatot;

    QStringList getTierList() const {
        return tierList;
    }

    /* Show player events, sort by tier, show timestamps */
    bool showPEvents;
    bool sortBT;
    bool showTS;
public slots:
    void errorFromNetwork(int errnum, const QString &error);
    void connected();
    void disconnected();
    /* message received from the server */
    void messageReceived(const QString & mess);
    /* sends what's in the line edit */
    void sendText();
    void playerLogin(const PlayerInfo &p);
    void playerReceived(const PlayerInfo &p);
    void teamChanged(const PlayerInfo &p);
    void announcementReceived(const QString &);
    void playerLogout(int);
    void sendRegister();
    /* sends the server a challenge notice */
    void sendChallenge(int id);
    /* removes the pointer to the challenge window when it is destroyed */
    void clearChallenge();
    /* sends the server a "Accept Challenge" notice */
    void acceptChallenge(int id);
    void refuseChallenge(int id);
    /* Display the info for that player */
    void seeInfo(int id);
    void seeInfo(QTreeWidgetItem *it);
    /* Challenge info by the server */
    void challengeStuff(const ChallengeInfo &c);
    /* Channels list */
    void channelCommandReceived(int command, int channel, QDataStream *stream);
    void channelsListReceived(const QHash<qint32, QString> &channels);
    void channelPlayers(int chanid, const QVector<qint32> &ids = QVector<qint32>());
    void addChannel(const QString &name, int id);
    void channelNameChanged(int id, const QString &name);
    void removeChannel(int id);
    void leaveChannelR(int index);
    void leaveChannel(int id);
    void itemJoin(QListWidgetItem *);
    void lineJoin();
    void firstChannelChanged(int tabindex);
    /* battle... */
    void battleStarted(int battleid, int id, const TeamBattle &team, const BattleConfiguration &conf, bool doubles);
    void battleStarted(int battleid, int id1, int id2);
    void battleFinished(int battleid, int res, int winner, int loser);
    void battleCommand(int battleid, const QByteArray&command);
    void saveBattleLogs(bool save);
    void animateHpBar(bool animate);
    void playMusic(bool music);
    void changeMusicFolder();
    void changeBattleLogFolder();
    void forfeitBattle(int);
    void watchBattleOf(int);
    void watchBattleRequ(int);
    void watchBattle(const QString &name0, const QString &name1, int battleId, bool doubles);
    void spectatingBattleMessage(int battleId, const QByteArray &command);
    void stopWatching(int battleId);
    void battleListActivated(QTreeWidgetItem* it);
    void loadTeam();
    /* A popup that asks for the pass */
    void askForPass(const QString &salt);
    /* When someone is kicked */
    void playerKicked(int,int);
    void playerBanned(int,int);
    /* When you kick someone */
    void kick(int);
    void ban(int);
    void tempban(int, int);
    /* PM */
    void startPM(int);
    void removePM(int);
    void PMReceived(int, const QString);
    /* CP */
    void controlPanel(int);
    void setPlayer(const UserInfo &ui);
    void requestBan(const QString &name);
    void requestTempBan(const QString &name, int time);
    /* Ranking */
    void seeRanking(int);
    /* Away... */
    void awayChanged(int id, bool away);
    void goAway(int away);
    void goAwayB(bool away) {
        goAway(away);
    }
    void changeButtonStyle(bool old);
    void showTeam(bool);
    void enableLadder(bool);
    void sortPlayersCountingTiers(bool);
    void showPlayerEvents(bool);
    void showTimeStamps(bool);
    void showTimeStamps2(bool);
    void ignoreServerVersion(bool);
    void versionDiff(const QString &a, const QString &b);
    void tierListReceived(const QString&);
    void changeTier();
    void openBattleFinder();
    void findBattle(const FindBattleData&);
    /* Ignored */
    void removeIgnore(int);
    void ignore(int);
    /* Teambuilder slots */
    void openTeamBuilder();
    void changeTeam();
    /* Automatic removal of players in memory */
    void fadeAway();
    void registerPermPlayer(int id);
signals:
    void done();
    void userInfoReceived(const UserInfo &ui);
protected:
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
private:
    TrainerTeam *myteam;
    QString mynick;

    /* GUI */
    /* Main chat */
    QScrollDownTextEdit *mychat;
    /* PMs */
    QHash<int, PMWindow*> mypms;
    /* Line the user types in */
    QLineEdit *myline;
    QLabel *announcement;
    /* Where players are displayed */
    QStackedWidget *playersW, *battlesW;
    QTabWidget *mainChat;
    QListWidget *channels;
    QLineEdit *channelJoin;
    /* Button to exit */
    QPushButton *myexit;
    /* Button to send text */
    QPushButton *mysender;
    /* Button to register a password */
    QPushButton *myregister;
    /* Button to find a battle */
    QPushButton *findMatch;

    /*Channels */
    QHash<qint32, QString> channelNames;
    QHash<QString, qint32> channelByNames;
    QHash<qint32, Channel *> mychannels;
    /* Ignore */
    QList<int> myIgnored;

    /* Challenge windows , to emit or to receive*/
    QSet<BaseChallengeWindow *> mychallenges;
    QPointer<BattleFinder> myBattleFinder;
    QHash<int, BaseBattleWindow* > mySpectatingBattles;
    QHash<int, BattleWindow* > mybattles;
    QAction *goaway;

    bool findingBattle;
    int _mid;

    QString serverVersion;

    QPointer<QMenuBar> mymenubar;
    QPointer<QMenu> mytiermenu;
    QList<QAction*> mytiers;
    QStringList tierList;
    /* You can call the teambuilder from here too */
    QPointer<QMainWindow> myteambuilder;

    QPointer<ControlPanel> myCP;
    QPointer<RankingDialog> myRanking;

    QHash<int, PlayerInfo> myplayersinfo;
    /* Players scheduled for deletion are put here */
    QHash<int, int> fade;
    /* Players which we have PMed are supposed to be kept in memory until they
       log out for real */
    QSet<int> pmedPlayers;
    QHash<QString, int> mynames;

    QHash<qint32, Battle> battles;

    /* Network Relay */
    Analyzer myrelay;
    Analyzer & relay();

    PlayerInfo & playerInfo(int id);
    void updateState(int player);
    /* Returns the challenge window displaying that player or NULL otherwise */
    BaseChallengeWindow * getChallengeWindow(int player);
    void closeChallengeWindow(BaseChallengeWindow *c);

    void initRelay();
    void changeTierChecked(const QString &newtier);
};

class BattleFinder : public QWidget
{
    Q_OBJECT
public:
    BattleFinder(QWidget *parent = NULL);
public slots:
    void throwChallenge();
    void changeEnabled();
signals:
    void findBattle(const FindBattleData&);
private:
    QCheckBox *sameTier, *rated, *rangeOn, *doubles;
    QCheckBox *clauses[ChallengeInfo::numberOfClauses];
    QLineEdit *range;
};

#endif // CLIENT_H
