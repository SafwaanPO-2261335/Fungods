#include "Level.h"

Level::Level(std::string group, std::string name, Uint32 event) {
    std::string buffer;
    size_t space;
    std::string key;
    std::string val;

    m_paths.push_back(new Path());
    std::vector<Wave*> waves;
    waves.push_back(new Wave(event));
    m_path_waves.push_back(waves);

    std::ifstream pathFile(std::filesystem::current_path().parent_path() / "Levels" / group / name / "paths.fgl");
    while (getline(pathFile, buffer)) {
        space = buffer.find(' ');
        key = buffer.substr(0, space);
        val = buffer.substr(space + 1, buffer.size() - space);

        if (key == "P") {
            m_paths.push_back(new Path());

            std::vector<Wave*> waves;
            waves.push_back(new Wave(event));
            m_path_waves.push_back(waves);
        }
        else if (key == "L") {
            size_t space2 = val.find(':');
            float x = stof(val.substr(0, space2));
            float y = stof(val.substr(space2 + 1, val.size() - space2));

            m_paths.back()->addLoc(Location(x, y));
        }
    }
    pathFile.close();

    int path_index = 0;

    std::ifstream waveFile(std::filesystem::current_path().parent_path() / "Levels" / group / name / "waves.fgl");
    while (getline(waveFile, buffer)) {
        space = buffer.find(' ');
        key = buffer.substr(0, space);
        val = buffer.substr(space + 1, buffer.size() - space);

        if (key == "W") {
            m_path_waves.at(path_index).push_back(new Wave(event));
        }
        else if (key == "D") {
            m_path_waves.at(path_index).back()->addDelay(stoi(val));
        }
        else if (key == "S") {
            DEBUG m_path_waves.size();
            m_path_waves.at(path_index).back()->addBacteria(val, m_paths.at(path_index));
        }
        else if (key == "P") {
            path_index++;
            if (path_index >= m_paths.size())
                path_index = 0;
        }
    }
    waveFile.close();
}

void Level::live() {
    for (auto wave : m_path_waves) {
        if (m_currentWave >= wave.size())
            continue;
        else if (wave.at(m_currentWave)->isDead()) {
            m_living = false;
            m_currentWave += 1;
            std::cout << "Wave Complete!\n";
        }
        else if (m_living) {
            wave.at(m_currentWave)->live();
        }
    }
}

bool Level::relive() {
    m_living = true;
    return m_currentWave < m_path_waves.at(0).size();
}