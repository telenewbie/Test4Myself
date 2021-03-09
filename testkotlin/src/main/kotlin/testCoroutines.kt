import javax.xml.bind.JAXBElement
import kotlinx.coroutines.flow.Flow
fun main(){
    val ints = sequence {
        (1..3).forEach {
            yield(it)
        }
    }
    val intFlow = flow {
        (1..3).forEach {
            emit(it)
            delay(100)
        }
    }


}