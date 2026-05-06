#include "CsvCarRepository.h"
#include <fstream>
#include <sstream>
#include <algorithm>

CsvCarRepository::CsvCarRepository(const std::string& filePath)
    : m_filePath(filePath) {}

// ── helpers ──────────────────────────────────────────────────────────────────
static std::string escapeCsv(const std::string& s) {
    // Owijamy w cudzysłów jeśli zawiera przecinek lub cudzysłów
    if (s.find_first_of(",\"") == std::string::npos) return s;
    std::string out = "\"";
    for (char c : s) { if (c == '"') out += '"'; out += c; }
    out += '"';
    return out;
}

static std::string unescapeCsv(const std::string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        std::string inner = s.substr(1, s.size() - 2);
        std::string out;
        for (size_t i = 0; i < inner.size(); ++i) {
            if (inner[i] == '"' && i + 1 < inner.size() && inner[i+1] == '"') ++i;
            out += inner[i];
        }
        return out;
    }
    return s;
}

// Prosty parser CSV obsługujący pola w cudzysłowach
static std::vector<std::string> parseCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string cur;
    bool inQ = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (inQ && i + 1 < line.size() && line[i+1] == '"') { cur += '"'; ++i; }
            else inQ = !inQ;
        } else if (c == ',' && !inQ) {
            fields.push_back(cur); cur.clear();
        } else {
            cur += c;
        }
    }
    fields.push_back(cur);
    return fields;
}

// ── ICarRepository ────────────────────────────────────────────────────────────
void CsvCarRepository::add(const Car& car) {
    m_cars.push_back(car);
    if (car.id >= m_nextId) m_nextId = car.id + 1;
}

void CsvCarRepository::remove(int id) {
    m_cars.erase(
        std::remove_if(m_cars.begin(), m_cars.end(),
                       [id](const Car& c){ return c.id == id; }),
        m_cars.end());
}

std::vector<Car> CsvCarRepository::getAll() const { return m_cars; }

int CsvCarRepository::nextId() const { return m_nextId; }

bool CsvCarRepository::load() {
    std::ifstream f(m_filePath);
    if (!f.is_open()) return false;          // brak pliku nie jest błędem przy pierwszym uruchomieniu

    m_cars.clear();
    m_nextId = 1;
    std::string line;
    std::getline(f, line);                   // pomiń nagłówek

    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto cols = parseCsvLine(line);
        if (cols.size() < 8) continue;

        Car c;
        c.id          = std::stoi(cols[0]);
        c.brand       = unescapeCsv(cols[1]);
        c.model       = unescapeCsv(cols[2]);
        c.year        = std::stoi(cols[3]);
        c.mileage     = std::stoi(cols[4]);
        c.price       = std::stod(cols[5]);
        c.description = unescapeCsv(cols[6]);
        c.imagePath   = unescapeCsv(cols[7]);

        m_cars.push_back(c);
        if (c.id >= m_nextId) m_nextId = c.id + 1;
    }
    return true;
}

bool CsvCarRepository::save() const {
    std::ofstream f(m_filePath);
    if (!f.is_open()) return false;

    f << "id,brand,model,year,mileage,price,description,imagePath\n";
    for (const auto& c : m_cars) {
        f << c.id << ","
          << escapeCsv(c.brand) << ","
          << escapeCsv(c.model) << ","
          << c.year << ","
          << c.mileage << ","
          << c.price << ","
          << escapeCsv(c.description) << ","
          << escapeCsv(c.imagePath) << "\n";
    }
    return true;
}
