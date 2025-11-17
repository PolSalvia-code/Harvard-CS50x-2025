
num_words = 1
num_sentences = 0
total_letters = 0
Text = input("Text : ")
len_Text = len(Text)
for i in range(len_Text):
    if Text[i] == ' ':
        num_words += 1
    elif Text[i] == '.' or Text[i] == '!' or Text[i] == '?':
        num_sentences += 1

    elif Text[i].isalpha():
        total_letters += 1


L = (total_letters / num_words) * 100
S = (num_sentences / num_words) * 100

index = round(0.0588 * L - 0.296 * S - 15.8)

if index < 1:
    print("Before Grade 1")

elif index >= 16:
    print("Grade 16+")

else:
    print(f"Grade {index}")
