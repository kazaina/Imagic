import Train as tr

list = []

s = int(input("성별을 입력해주세요\n1. 남성\t2. 여성\ninput >> "))
list.append(s-1)
while(1):
    age = int(input("귀하의 나이를 입력해주세요\ninput >> "))
    if age < 20:
        print("20살 이상만 입력해주세요.\n")
        continue
    elif age >= 20 and age < 23:
        list.append(0)
        break
    elif age >= 23 and age < 26:
        list.append(1)
        break
    elif age >= 26 and age < 30:
        list.append(2)
        break
    elif age >= 30:
        list.append(3)
        break

