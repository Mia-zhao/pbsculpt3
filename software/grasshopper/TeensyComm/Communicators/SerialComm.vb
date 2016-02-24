Imports System.Windows.Threading
Imports TeensyComm.Parsing

Namespace Communicators


    ''' <summary>
    ''' Performs reading and writing of TMessages over SerialPort
    ''' </summary>
    Public Class SerialComm

#Region "Fields"

        Private _COM As String = String.Empty
        Private _receivedMessages As New List(Of TMessage)
        Private _sendMessages As New List(Of TMessage)
        Private _interval As Integer = 0
        Private _timeout As Integer = 0
        Private StopBool As Boolean = False

        Private _port As IO.Ports.SerialPort = Nothing
        Private WithEvents St As New System.Timers.Timer(1000)

        Private T As Threading.Thread = Nothing
        Private ThisDisp As Dispatcher = Dispatcher.CurrentDispatcher

        Private MessOut As New List(Of TMessage)
        Private Event GotMessage(sender As SerialComm, e As TeensyEventArgs)

        ''' <summary>
        ''' TeensyEventArgs holds all the messages which arrived since last timer tick
        ''' </summary>
        ''' <param name="sender"></param>
        ''' <param name="e"></param>
        Public Event MessageReceived(sender As SerialComm, e As TeensyEventArgs)

        ''' <summary>
        ''' That's intended to indicate a device which is sending SOME messages, but they're not recognized as Teensy Messaged by the parser
        ''' </summary> 
        ''' <param name="sender"></param>
        ''' <param name="e"></param>
        ''' <remarks></remarks>
        Public Event IncorrectMessageReceived(sender As SerialComm, e As TeensyEventArgs)
        Public Event TimeoutReached(sender As SerialComm, e As TeensyEventArgs)

#End Region

        ''' <summary>
        ''' Default and only constructor
        ''' </summary>
        ''' <param name="COMName">COM port to communicate with</param>
        ''' <param name="Interval">Time interval at which the current message stack is accessible via MessageReceived event</param>
        ''' <param name="Timeout">Timeout value</param>
        Sub New(COMName As String, Interval As Integer, Optional Timeout As Integer = -1)
            _COM = COMName
            _timeout = Timeout
            _interval = Interval
            St.AutoReset = True
        End Sub

        ''' <summary>
        ''' Start communication
        ''' </summary>
        Public Sub Start()
            StopBool = False
            _port = My.Computer.Ports.OpenSerialPort(_COM)
            _port.DiscardNull = True
            _port.NewLine = "/"
            _port.ReadTimeout = -1
            _port.WriteTimeout = -1
            T = New Threading.Thread(AddressOf ReceiveSerialData)
            T.IsBackground = True
            T.Start()
            St.Start()
        End Sub

        ''' <summary>
        ''' Close communication and dispose stuff
        ''' </summary>
        Public Sub Close()
            StopBool = True
            St.Dispose()
        End Sub

        Public ReadOnly Property Name As String
            Get
                Return _COM
            End Get
        End Property

        Public Property Interval As Integer
            Get
                Return _interval
            End Get
            Set(value As Integer)
                _interval = value
                St.Interval = value
            End Set
        End Property

        ''' <summary>
        ''' Puts messages to send in the stack
        ''' </summary>
        ''' <param name="Message"></param>
        Public Sub Send(Message As TMessage)
            _sendMessages.Add(Message)
        End Sub

        Private Sub ReceiveSerialData()
            Try
                Do
                    If _port Is Nothing Then Exit Do
                    If Not _port.IsOpen Then Exit Do
                    If StopBool And _port IsNot Nothing Then _port.Close() : Exit Sub

                    'send messages
                    For i As Integer = 0 To _sendMessages.Count - 1 Step 1
                        Dim b() As Byte = _sendMessages(i).ToBytes
                        _port.Write(b, 0, b.Length)
                    Next
                    _sendMessages.Clear()

                    'get messages
                    If _port.BytesToRead < 2 Then Continue Do
                    Dim buf(_port.BytesToRead - 1) As Byte
                    _port.Read(buf, 0, buf.Length - 1)
                    Dim mess As List(Of TMessage) = ListFromBytes(buf, 2)
                    _port.DiscardInBuffer()
                    If mess IsNot Nothing Then
                        If mess.Count > 0 Then
                            For i As Integer = 0 To mess.Count - 1 Step 1
                                If mess(i).IsValid Then
                                    _receivedMessages.Add(mess(i))
                                Else
                                    If ThisDisp IsNot Nothing Then
                                        ThisDisp.Invoke(AddressOf OnIncorrectReceived)
                                    End If
                                    _receivedMessages.Clear()
                                    Threading.Thread.Sleep(_interval)
                                    If StopBool And _port IsNot Nothing Then _port.Close() : Exit Sub
                                End If
                            Next
                        End If
                    End If
                Loop
            Catch ex As Exception

            Finally

            End Try

        End Sub

        Private Sub GetMessagesOut() Handles St.Elapsed
            For i As Integer = 0 To _receivedMessages.Count - 1 Step 1
                MessOut.Add(_receivedMessages(i).Duplicate)
            Next
            If ThisDisp IsNot Nothing Then ThisDisp.Invoke(AddressOf OnMessageReceived)
            _receivedMessages.Clear()
        End Sub

        Private Sub OnMessageReceived()
            Dim tmp As New List(Of TMessage)
            For i As Integer = 0 To MessOut.Count - 1 Step 1
                tmp.Add(MessOut(i).Duplicate)
            Next

            RaiseEvent MessageReceived(Me, New TeensyEventArgs(_COM, tmp))
            MessOut.Clear()
        End Sub

        Private Sub OnIncorrectReceived()
            RaiseEvent IncorrectMessageReceived(Me, New TeensyEventArgs(_COM, _receivedMessages))
        End Sub

        Private Sub OnTimeout()
            RaiseEvent TimeoutReached(Me, New TeensyEventArgs(_COM, _receivedMessages))
        End Sub

    End Class

End Namespace
