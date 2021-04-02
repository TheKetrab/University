package narzedzia.wyjatki;

public class WyjatekONP extends Exception {

    private String reason;

    public WyjatekONP(String reason) {
        this.reason = reason;
    }

    @Override
    public String toString() {
        return reason;
    }
}
