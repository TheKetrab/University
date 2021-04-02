public class InterpreterException extends Exception {

    private String reason;

    public InterpreterException(String reason)
    {
        this.reason = reason;
    }

    @Override
    public String toString()
    {
        return reason;
    }


}
