import sys
import random

surnames = ["Иванов", "Петров", "Сидоров", "Смирнов", "Кузнецов", "Попов", "Васильев", "Морозов", "Новиков"]
names = ["Алексей", "Иван", "Сергей", "Дмитрий", "Михаил", "Андрей", "Артем", "Максим"]
patronyms = ["Иванович", "Петрович", "Сергеевич", "Алексеевич", "Дмитриевич", "Андреевич"]

# Генератор тестового файла с клиентами и операциями (не менее 50 строк)
def generate(filename="test_data_50lines.txt", num_clients=12):
    lines = []
    for cid in range(101, 101 + num_clients):
        fio = f"{random.choice(surnames)}\t{random.choice(names)}\t{random.choice(patronyms)}"
        tx_count = random.randint(3, 5)
        lines.extend([str(cid), fio, str(tx_count)])

        for _ in range(tx_count):
            day = random.randint(1, 28)
            month = random.randint(1, 12)
            hour = random.randint(8, 20)
            minute = random.randint(0, 59)
            ttype = random.choice(["Приход", "Расход"])
            amt = round(random.uniform(100.0, 15000.0), 2)
            lines.append(f"{day:02d}.{month:02d}.24\t{hour:02d}:{minute:02d}:00\t{ttype}\t{amt:.2f}")

    with open(filename, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")

    print(f"Сгенерировано {len(lines)} строк в файл '{filename}'")

if __name__ == "__main__":
    out_file = sys.argv[1] if len(sys.argv) > 1 else "test_data_50lines.txt"
    generate(out_file)
