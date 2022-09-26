#include <QApplication>
#include <QTimer>
#include <QStyleFactory>
#include <QCommandLineParser>

#include "spiritmanager.hpp"
#include "termcolor.hpp"

static void info() {
    std::cout << termcolor::bold << "Spirit 👻"
              << termcolor::reset
              << " "
              << termcolor::underline
              << SPIRIT_VERSION
              << termcolor::reset
              << " commit-"
              << SPIRIT_COMMIT
              << " (build "
              << SPIRIT_BUILD
              << "),"
              << " The Twenty First Century Window Sitter."
              << "\n"
              << "Copyright (C) 2021, "
              << termcolor::magenta
              << termcolor::bold
              << "D. Antony J.R <antonyjr@pm.me>."
              << termcolor::reset
              << "\n\n";
}

static void usage(const char *prog) {
    info();

    std::cout << termcolor::bold
              << "Usage: " << prog << " <SUBCOMMAND> [OPTIONS]\n"
              << "       " << prog << " load <LOCAL SPIRIT FILE> [OPTIONS]\n"
              << "       " << prog << " set-action <ACTION NAME> [OPTIONS]\n"
              << "       " << prog << " set-property <Property=Value> ... [OPTIONS]\n"
              << "       " << prog << " add-quirk name=<ProgramName> yoffset=<int> ... [OPTIONS]\n"
              << termcolor::reset
              << "\n\n";

    std::cout << termcolor::bold
              << "SUBCOMMAND:\n\n"
              << termcolor::reset
              << " init           " << "\t" << "Initialize spirit daemon.\n"
              << " load           " << "\t" << "Load a spirit from local file.\n"
              << " info           " << "\t" << "Show info on current loaded spirit file.\n"
              << " set-action     " << "\t" << "Set current action of the loaded spirit.\n"
              << " list-actions   " << "\t" << "List all actions of the loaded spirit.\n"
              << " show-action    " << "\t" << "Show current action.\n"
              << " quirks         " << "\t" << "Show Quirks list.\n"
              << " add-quirk      " << "\t" << "Add a Quirk.\n"
              << " remove-quirk   " << "\t" << "Remove a Quirk.\n"
              << " property       " << "\t" << "Show current action properties.\n"
              << " set-property   " << "\t" << "Set properties for current spirit.\n"
              << " reset-property " << "\t" << "Reset properties to default.\n"
              << " gh-load        " << "\t" << "Load a spirit file from github repo.\n"
              // << " renit     " << "\t" << "Restart spirit daemon.\n"
              << " deinit         " << "\t" << "Quit spirit daemon.\n"
              << " get-port       " << "\t" << "Get the port that spirit daemon uses.\n\n";

    std::cout << termcolor::bold
              << "OPTIONS [init]:\n\n"
              << termcolor::reset
              << " -d,--debug " << "\t" << "Enable debug messages.\n\n";
}

int main(int ac, char **av) {
    if(ac == 1) {
        // TODO: Use this hint as the gui mode.
        usage(av[0]);
        return 0;
    }

    QApplication app(ac, av);
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QCommandLineParser parser;

    parser.addPositionalArgument("subcommand",
                                 QCoreApplication::translate("main", "subcommand"));

    QCommandLineOption helpOption(
        QStringList() << "h"
        << "help",
        QCoreApplication::translate("main", "Show help."));
    parser.addOption(helpOption);

    QCommandLineOption debugOption(
        QStringList() << "d"
        << "debug",
        QCoreApplication::translate("main", "Enable debug messages."));
    parser.addOption(debugOption);

    parser.process(app);

    if(parser.isSet(helpOption)) {
        usage(av[0]);
        return 0;
    }

    const QStringList args = parser.positionalArguments();
    if(args.size() == 0) {
        usage(av[0]);
        return 0;
    }

    const QString subcommand = args.at(0).toLower();
    SpiritManager manager(&app);

    if(subcommand == "init") {
        info();

        QObject::connect(&manager, &SpiritManager::initialized,
        [&app](bool inits) {
            if (!inits) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Initialize Spirit."
                          << termcolor::reset
                          << "\n";
                app.exit(-1);
                return;
            } else {
                std::cout << termcolor::bold
                          << termcolor::yellow
                          << "Spirit Initialized Successfully."
                          << termcolor::reset
                          << "\n";
            }
            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, &manager, &SpiritManager::init);

        timer.start();
        return app.exec();

    } /*else if(subcommand == "reinit") {
	   info();

	   QObject::connect(&manager, &SpiritManager::initialized,
	   [&app](bool inits) {
		   	if (!inits) {
		std::cout << termcolor::bold
				  << termcolor::red
				  << "Failed to Re-Initialize Spirit."
				  << termcolor::reset
				  << "\n";

				  app.exit(-1);
				  return;
		} else {
		   std::cout << termcolor::bold
			  << termcolor::yellow
			  << "Spirit Re-Initialized Successfully."
			  << termcolor::reset
			  << "\n";
		}

	   app.quit();
	   });

	   QTimer timer;
	   timer.setSingleShot(100);
	   QObject::connect(&timer, &QTimer::timeout, &manager, &SpiritManager::reinit);

	   timer.start();
	   return app.exec();
	}*/ else if(subcommand == "deinit") {
        info();

        QObject::connect(&manager, &SpiritManager::deinitialized,
        [&app](bool deinits) {
            if (!deinits) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to De-Initialize Spirit."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            } else {
                std::cout << termcolor::bold
                          << termcolor::yellow
                          << "Spirit De-Initialized Successfully."
                          << termcolor::reset
                          << "\n";
            }


            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, &manager, &SpiritManager::deinit);
        timer.start();

        return app.exec();
    } else if(subcommand == "get-port") {
        std::cout << manager.getPort() << "\n";
    } else if(subcommand == "load") {
        if (args.size() < 2) {
            usage(av[0]);
            return 0;
        }

        info();
        QObject::connect(&manager, &SpiritManager::loaded,
        [&app](bool loads) {
            if (!loads) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Load Spirit File."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            } else {
                std::cout << termcolor::bold
                          << termcolor::yellow
                          << "Spirit Loaded Successfully."
                          << termcolor::reset
                          << "\n";
            }


            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, [&args, &manager]() {
            manager.load(args.at(1));
        });
        timer.start();

        return app.exec();

    } else if(subcommand == "quirks") {
        info();
        QObject::connect(&manager, &SpiritManager::quirks,
        [&app](bool success, QJsonObject quirks) {
            if (!success) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Get Quirks."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            } else {

                Q_UNUSED(quirks.take("status"));
                auto keys = quirks.keys();
                for (auto key : keys) {
                    auto obj = quirks[key].toObject();

                    int x = obj["xoffset"].toInt(),
                        y = obj["yoffset"].toInt();
                    int x2 = obj["bottomXOffset"].toInt(),
                        y2 = obj["bottomYOffset"].toInt();

                    QString vname = obj["visibleName"].toString();

                    std::cout << termcolor::bold
                              << termcolor::yellow
                              << key.toStdString()
                              << termcolor::reset
                              << ": "
                              << "XOffset = "
                              << x
                              << " | YOffset = "
                              << y
                              << " | Bottom XOffset = "
                              << x2
                              << " | Bottom YOffset = "
                              << y2;

                    if (vname.isEmpty()) {
                        std::cout << "\n";
                    } else {
                        std::cout << " | Visible Name = "
                                  << vname.toStdString()
                                  << "\n";
                    }
                }

                std::cout << "\n";
            }


            app.quit();

        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, &manager, &SpiritManager::getQuirks);
        timer.start();

        return app.exec();
    } else if(subcommand == "add-quirk") {
        info();
        QObject::connect(&manager, &SpiritManager::quirks,
        [&app](bool success, QJsonObject quirks) {
            if (!success) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Get Quirks."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            } else {

                Q_UNUSED(quirks.take("status"));
                auto keys = quirks.keys();
                for (auto key : keys) {
                    auto obj = quirks[key].toObject();

                    int x = obj["xoffset"].toInt(),
                        y = obj["yoffset"].toInt();
                    int x2 = obj["bottomXOffset"].toInt(),
                        y2 = obj["bottomYOffset"].toInt();

                    QString vname = obj["visibleName"].toString();

                    std::cout << termcolor::bold
                              << termcolor::yellow
                              << key.toStdString()
                              << termcolor::reset
                              << ": "
                              << "XOffset = "
                              << x
                              << " | YOffset = "
                              << y
                              << " | Bottom XOffset = "
                              << x2
                              << " | Bottom YOffset = "
                              << y2;

                    if (vname.isEmpty()) {
                        std::cout << "\n";
                    } else {
                        std::cout << " | Visible Name = "
                                  << vname.toStdString()
                                  << "\n";
                    }
                }

                std::cout << "\n";
            }


            app.quit();

        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, [&app, &manager, &args]() {
            QJsonObject quirk { };
            for(auto i = 1; i < args.size(); ++i) {
                auto opt = args.at(i);
                auto st = opt.split("=");
                if (st.size() != 2) {
                    std::cout << termcolor::bold
                              << termcolor::red
                              << "Syntax Error, Failed to Add Quirk."
                              << termcolor::reset
                              << "\n";

                    app.exit(-1);
                    return;

                }

                if (st.at(0) == "name") {
                    quirk.insert(st.at(0), st.at(1));
                } else if(st.at(0) == "visibleName") {
                    quirk.insert(st.at(0), st.at(1));
                } else {
                    bool ok = false;
                    int v = st.at(1).toInt(&ok);

                    if (ok) {
                        quirk.insert(st.at(0), v);
                    }
                }
            }

            manager.addQuirk(quirk);

        });
        timer.start();

        return app.exec();
    } else if(subcommand == "remove-quirk") {
        info();
        QObject::connect(&manager, &SpiritManager::quirks,
        [&app](bool success, QJsonObject quirks) {
            if (!success) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Get Quirks."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            } else {

                Q_UNUSED(quirks.take("status"));
                auto keys = quirks.keys();
                for (auto key : keys) {
                    auto obj = quirks[key].toObject();

                    int x = obj["xoffset"].toInt(),
                        y = obj["yoffset"].toInt();
                    int x2 = obj["bottomXOffset"].toInt(),
                        y2 = obj["bottomYOffset"].toInt();

                    QString vname = obj["visibleName"].toString();

                    std::cout << termcolor::bold
                              << termcolor::yellow
                              << key.toStdString()
                              << termcolor::reset
                              << ": "
                              << "XOffset = "
                              << x
                              << " | YOffset = "
                              << y
                              << " | Bottom XOffset = "
                              << x2
                              << " | Bottom YOffset = "
                              << y2;

                    if (vname.isEmpty()) {
                        std::cout << "\n";
                    } else {
                        std::cout << " | Visible Name = "
                                  << vname.toStdString()
                                  << "\n";
                    }
                }
                std::cout << "\n";
            }


            app.quit();

        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, [&app, &manager, &args]() {
            if (args.size() != 2) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Invalid Argument Count."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;

            }

            manager.removeQuirk(args.at(1));
        });
        timer.start();

        return app.exec();
    }

    else if(subcommand == "property") {
        info();
        QObject::connect(&manager, &SpiritManager::properties,
        [&app](bool success, QJsonObject props) {
            if (!success) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Get Spirit Properties."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            } else {

                auto keys = props.keys();
                for (auto key : keys) {
                    std::cout << termcolor::bold
                              << termcolor::yellow
                              << key.toStdString()
                              << termcolor::reset
                              << ": "
                              << (props[key].isString() ?
                                  props[key].toString().toStdString() :
                                  QString::number(props[key].toInt()).toStdString())
                              << "\n";
                }

                std::cout << "\n";
            }


            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, &manager, &SpiritManager::getProperties);
        timer.start();

        return app.exec();
    } else if(subcommand == "set-property") {
        if (args.size() < 2) {
            usage(av[0]);
            return 0;
        }

        info();

        QObject::connect(&manager, &SpiritManager::updatedProperties,
        [&app](bool success, QJsonObject props) {
            if (!success) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Set Spirit Properties."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            } else {

                std::cout << termcolor::bold
                          << "Properties Updated Successfully."
                          << termcolor::reset
                          << "\n\n";

                auto keys = props.keys();
                for (auto key : keys) {
                    std::cout << termcolor::bold
                              << termcolor::yellow
                              << key.toStdString()
                              << termcolor::reset
                              << ": "
                              << (props[key].isString() ?
                                  props[key].toString().toStdString() :
                                  QString::number(props[key].toInt()).toStdString())
                              << "\n";
                }

                std::cout << "\n";
            }


            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, [&app, &manager, &args]() {
            QJsonObject updateProps { };
            for(auto i = 1; i < args.size(); ++i) {
                auto opt = args.at(i);
                auto st = opt.split("=");
                if (st.size() != 2) {
                    std::cout << termcolor::bold
                              << termcolor::red
                              << "Syntax Error, Failed to Set Spirit Properties."
                              << termcolor::reset
                              << "\n";

                    app.exit(-1);
                    return;

                }

                if (st.at(0) == "position") {
                    updateProps.insert(st.at(0), st.at(1));
                } else {
                    bool ok = false;
                    int v = st.at(1).toInt(&ok);

                    if (ok) {
                        updateProps.insert(st.at(0), v);
                    }
                }
            }

            manager.setProperties(updateProps);
        });
        timer.start();

        return app.exec();
    } else if(subcommand == "reset-property") {
        info();
        QObject::connect(&manager, &SpiritManager::properties,
        [&app](bool success, QJsonObject props) {
            if (!success) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Reset Spirit Properties."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            } else {

                auto keys = props.keys();
                for (auto key : keys) {
                    std::cout << termcolor::bold
                              << termcolor::yellow
                              << key.toStdString()
                              << termcolor::reset
                              << ": "
                              << (props[key].isString() ?
                                  props[key].toString().toStdString() :
                                  QString::number(props[key].toInt()).toStdString())
                              << "\n";
                }

                std::cout << "\n";
            }


            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, &manager, &SpiritManager::resetProperty);
        timer.start();

        return app.exec();

    } else if(subcommand == "info") {
        info();
        QObject::connect(&manager, &SpiritManager::loadedSpiritInfo,
        [&app](bool success, QJsonObject info) {
            if (!success) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Get Spirit Info."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            } else {

                auto keys = info.keys();
                for (auto key : keys) {
                    std::cout << termcolor::bold
                              << termcolor::yellow
                              << key.toStdString()
                              << termcolor::reset
                              << ": "
                              << (info[key].isString() ?
                                  info[key].toString().toStdString() :
                                  QString::number(info[key].toInt()).toStdString())
                              << "\n";
                }

                std::cout << "\n";
            }


            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, &manager, &SpiritManager::getLoadedSpiritInfo);
        timer.start();

        return app.exec();
    } else if(subcommand == "set-action") {

        if (args.size() < 2) {
            usage(av[0]);
            return 0;
        }

        info();
        QObject::connect(&manager, &SpiritManager::action,
        [&app](QString action) {
            std::cout << termcolor::bold
                      << termcolor::yellow
                      << "Action("
                      << action.toStdString()
                      << ") set Successfully."
                      << termcolor::reset
                      << "\n";
            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, [&args, &manager]() {
            manager.setAction(args.at(1));
        });
        timer.start();

        return app.exec();
    } else if(subcommand == "list-actions") {
        info();

        QObject::connect(&manager, &SpiritManager::actions,
        [&app](QStringList actions) {
            if (actions.size() == 0) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Get Spirit Actions."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            }

            for(auto action : actions) {
                std::cout << termcolor::bold
                          << termcolor::yellow
                          << action.toStdString()
                          << termcolor::reset
                          << "\n";
            }

            std::cout << "\n";
            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, &manager, &SpiritManager::getActions);
        timer.start();

        return app.exec();

    } else if(subcommand == "show-action") {
        info();

        QObject::connect(&manager, &SpiritManager::action,
        [&app](QString action) {
            if (action.isEmpty()) {
                std::cout << termcolor::bold
                          << termcolor::red
                          << "Failed to Show Current Spirit Action."
                          << termcolor::reset
                          << "\n";

                app.exit(-1);
                return;
            }

            std::cout << termcolor::bold
                      << termcolor::yellow
                      << "Current Action: "
                      << termcolor::reset
                      << action.toStdString()
                      << "\n";
            app.quit();
        });

        QTimer timer;
        timer.setSingleShot(100);
        QObject::connect(&timer, &QTimer::timeout, &manager, &SpiritManager::getAction);
        timer.start();

        return app.exec();
    } else if(subcommand == "gh-load") {
        // TODO: Implement gh-load
    } else if(subcommand == "daemon") {
        manager.daemon();
    } else {
        usage(av[0]);
        return 0;
    }

    return manager.exec();
}
