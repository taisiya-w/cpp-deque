#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "algo.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <random>
#include <string>
#include <functional>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ApplyModel();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::ApplyModel() {
    auto it = deque_model_.iterator;
    ui -> list_widget -> clear();
    for (size_t i = 0; i < deque_model_.items.size(); i++) {
        QString item = QString("%1: %2").arg(i).arg(QString::fromStdString(deque_model_.items[i]));
        ui -> list_widget -> addItem(item);
    }
    ui -> list_widget -> addItem("end");

    ui -> btn_pop_back -> setDisabled(deque_model_.items.empty());
    ui -> btn_pop_front -> setDisabled(deque_model_.items.empty());
    ui -> btn_erase -> setDisabled(deque_model_.items.empty());
    ui -> txt_size -> setText(QString::number(deque_model_.items.size()));

    deque_model_.iterator = it;
    ApplyIterator();
}

void MainWindow::ApplyIterator() {
    auto index = std::distance(deque_model_.items.begin(), deque_model_.iterator);
    if (index >= 0 && index <= static_cast<int>(deque_model_.items.size())) {
        ui -> list_widget -> setCurrentRow(index);
    }

    bool at_end = deque_model_.iterator == deque_model_.items.end();
    bool at_start = deque_model_.iterator == deque_model_.items.begin();

    ui->btn_edit->setDisabled(at_end);
    ui->btn_erase->setDisabled(at_end);
    ui->btn_inc_iterator->setDisabled(at_end);
    ui->btn_dec_iterator->setDisabled(at_start);

    if (at_end) {
        ui -> txt_elem_content -> clear();
    } else {
        ui -> txt_elem_content -> setText(QString::fromStdString(*deque_model_.iterator));
    }
}

void MainWindow::SetRandomGen(const std::mt19937 &random_gen) {
    deque_model_.random_gen = random_gen;
    ApplyModel();
}

void MainWindow::on_btn_pop_back_clicked() {
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_back();
        deque_model_.iterator = deque_model_.items.begin();
    }
    ApplyModel();
}


void MainWindow::on_btn_push_back_clicked() {
    QString text = ui -> txt_elem_content -> text();
    if (!text.isEmpty()) {
        deque_model_.items.push_back(text.toStdString());
        deque_model_.iterator = deque_model_.items.begin();
    }
    ApplyModel();
}


void MainWindow::on_btn_clear_clicked() {
    deque_model_.items.clear();;
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}


void MainWindow::on_btn_erase_clicked() {
    if (deque_model_.iterator != deque_model_.items.end()) {
        deque_model_.iterator = deque_model_.items.erase(deque_model_.iterator);
        deque_model_.iterator = deque_model_.items.begin();
        ApplyModel();
    }
}


void MainWindow::on_btn_insert_clicked() {
    auto pos = deque_model_.iterator;
    deque_model_.iterator = deque_model_.items.insert(pos, ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}


void MainWindow::on_btn_dec_iterator_clicked() {
    if (deque_model_.iterator != deque_model_.items.begin()) {
        --deque_model_.iterator;
    }
    ApplyIterator();
}


void MainWindow::on_btn_inc_iterator_clicked() {
    if (deque_model_.iterator != deque_model_.items.end()) {
        ++deque_model_.iterator;
    }
    ApplyIterator();
}


void MainWindow::on_btn_begin_clicked() {
    deque_model_.iterator = deque_model_.items.begin();
    ApplyIterator();
}


void MainWindow::on_btn_end_clicked() {
    deque_model_.iterator = deque_model_.items.end();
    ApplyIterator();
}


void MainWindow::on_list_widget_currentRowChanged(int currentRow) {
    currentRow = std::clamp(currentRow, 0, static_cast<int>(deque_model_.items.size()));
    if (currentRow == static_cast<int>(deque_model_.items.size())) {
        deque_model_.iterator = deque_model_.items.end();
    } else {
        deque_model_.iterator = deque_model_.items.begin() + currentRow;
    }
    ApplyIterator();
}


void MainWindow::on_btn_edit_clicked() {
    if (deque_model_.iterator != deque_model_.items.end()) {
        *deque_model_.iterator = ui -> txt_elem_content -> text().toStdString();
        ApplyModel();
    }
}

void MainWindow::on_btn_resize_clicked() {
    bool ok = false;
    int new_it = ui -> txt_size -> text().toInt(&ok);

    if (!ok || new_it < 0 || new_it > 1000) {
        return;
    }
    deque_model_.items.resize(new_it);
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}


void MainWindow::on_btn_find_clicked() {
    QString target = ui -> txt_elem_content -> text();
    auto it = std::find(deque_model_.items.begin(), deque_model_.items.end(), target.toStdString());
    deque_model_.iterator = it;
    ApplyIterator();
}


void MainWindow::on_btn_count_clicked() {
    QString value = ui -> le_count -> text();
    int c = std::count(deque_model_.items.begin(), deque_model_.items.end(), value.toStdString());
    ui -> lbl_count -> setText(QString::number(c));
}


void MainWindow::on_btn_min_element_clicked() {
    if (deque_model_.items.empty()) {
        return;
    }
    auto it = std::min_element(deque_model_.items.begin(), deque_model_.items.end());
    deque_model_.iterator = it;
    ApplyIterator();
}


void MainWindow::on_btn_max_element_clicked() {
    if (deque_model_.items.empty()) {
        return;
    }
    auto it = std::max_element(deque_model_.items.begin(), deque_model_.items.end());
    deque_model_.iterator = it;
    ApplyIterator();
}


void MainWindow::on_btn_merge_sort_clicked() {
    deque_model_.items = MergeSort(deque_model_.items, std::less<std::string>());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}


void MainWindow::on_btn_merge_sOrT_clicked() {
    auto comp = [](const std::string& a, const std::string& b){
        return QString::fromStdString(a).compare(QString::fromStdString(b), Qt::CaseInsensitive) < 0;
    };
    deque_model_.items = MergeSort(deque_model_.items, comp);
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}


void MainWindow::on_btn_unique_clicked() {
    if (std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        auto del = std::unique(deque_model_.items.begin(), deque_model_.items.end());
        deque_model_.items.erase(del, deque_model_.items.end());
        ApplyModel();
    }
}


void MainWindow::on_btn_shuffle_clicked() {
    std::shuffle(deque_model_.items.begin(), deque_model_.items.end(), deque_model_.random_gen);
    ApplyModel();
}

void MainWindow::on_btn_reverse_clicked() {
    std::reverse(deque_model_.items.begin(), deque_model_.items.end());
    ApplyModel();
}


void MainWindow::on_btn_pop_front_clicked() {
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_front();
        deque_model_.iterator = deque_model_.items.begin();
    }
    ApplyModel();
}


void MainWindow::on_btn_push_front_clicked() {
    QString text = ui -> txt_elem_content -> text();
    if (!text.isEmpty()) {
        deque_model_.items.push_front(text.toStdString());
        deque_model_.iterator = deque_model_.items.begin();
    }
    ApplyModel();
}


void MainWindow::on_btn_lower_bound_clicked() {
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }
    QString text = ui -> txt_elem_content -> text();
    auto it = std::lower_bound(deque_model_.items.begin(), deque_model_.items.end(), text.toStdString());
    deque_model_.iterator = it;
    ApplyIterator();
}


void MainWindow::on_btn_upper_bound_clicked() {
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }
    QString text = ui -> txt_elem_content -> text();
    auto it = std::upper_bound(deque_model_.items.begin(), deque_model_.items.end(), text.toStdString());
    deque_model_.iterator = it;
    ApplyIterator();
}


void MainWindow::on_btn_tea_clicked() {
    static std::deque<std::string> tea {
        "Чай Лунцзин",
        "Эрл Грей",
        "Сенча",
        "Пуэр",
        "Дарджилинг",
        "Ассам",
        "Матча",
        "Ганпаудер",
        "Оолонг",
        "Лапсанг Сушонг"
    };

    deque_model_.items = tea;
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();

}


void MainWindow::on_btn_cakes_clicked() {
    static std::deque<std::string> cakes {
        "Красный бархат",
        "Наполеон",
        "Медовик",
        "Тирамису",
        "Прага",
        "Чизкейк",
        "Захер",
        "Эстерхази",
        "Морковный торт",
        "Чёрный лес",
    };
    deque_model_.items = cakes;
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

