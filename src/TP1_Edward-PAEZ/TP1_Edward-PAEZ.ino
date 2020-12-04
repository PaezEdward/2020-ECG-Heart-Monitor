
const int ledPin5 = 5;
const int ledPin18 = 18;
const int ledPin19 = 19;

const int BoutonPin34 = 34;

void setup() {
  // put your setup code here, to run once:
  pinMode (ledPin5, OUTPUT);
  pinMode (ledPin18, OUTPUT);
  pinMode (ledPin19, OUTPUT);
  pinMode (BoutonPin34, INPUT);
}

void loop() {

  if (BoutonPin34 == 0)

  {
    digitalWrite (ledPin5, HIGH);
    digitalWrite (ledPin18, HIGH);
    digitalWrite (ledPin19, HIGH);
    delay(500);
    digitalWrite (ledPin5, LOW);
    digitalWrite (ledPin18, LOW);
    digitalWrite (ledPin19, LOW);
    delay(500);
  }
  else
  {
    digitalWrite (ledPin5, HIGH);
    delay(500);
    digitalWrite (ledPin5, LOW);
    delay(500);
    digitalWrite (ledPin18, HIGH);
    delay(500);
    digitalWrite (ledPin18, LOW);
    delay(500);
    digitalWrite (ledPin19, HIGH);
    delay(500);
    digitalWrite (ledPin19, LOW);
    delay(500);
  }
}
