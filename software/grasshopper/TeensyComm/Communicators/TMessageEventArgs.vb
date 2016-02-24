Namespace Communicators

    ''' <summary>
    ''' Contains messages received as well as name of the sender
    ''' </summary>
    Public Class TeensyEventArgs

        Inherits System.EventArgs

        Private nam As String = String.Empty
        Private mes As New List(Of TMessage)

        Sub New(TeensyName As String, Message As List(Of TMessage))
            nam = TeensyName
            mes = Message
        End Sub

        Public ReadOnly Property Messages As List(Of TMessage)
            Get
                Return mes
            End Get
        End Property

        Public ReadOnly Property PortName As String
            Get
                Return nam
            End Get
        End Property

    End Class

End Namespace


