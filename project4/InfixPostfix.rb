# 
# InfixPostfix class contains methods for infix to postfix conversion and
# postfix expression evaluation.
#
# By Lawrence Longfield O'Boyle II && Megan Kelly Maher
class InfixPostfix
  
  # converts the infix expression string to postfix expression and returns it
  def infixToPostfix(exprStr)
		postfixStr = ""
		s = Array.new
		tokens = exprStr.split
		tokens.each do |token|
			if operand?(token)
				postfixStr << " " << token
			elsif leftParen?(token)
				s.push(token)
			elsif operator?(token)
				while !s.empty? && (stackPrecedence(s.last) >= inputPrecedence(token)) do
					postfixStr << " " << s.pop
				end
				s.push(token)
			elsif rightParen?(token)
				while !leftParen?(s.last)
					postfixStr << " " << s.pop
				end
				s.pop
			end
		end

		while !s.empty?
			if !rightParen?(s.last) && !leftParen?(s.last)
				postfixStr << " " << s.pop
			else
				s.pop
			end
		end
		return postfixStr.lstrip
  end

  # evaluate the postfix string and returns the result
  def evaluatePostfix(exprStr)
		s = Array.new
		tokens = exprStr.split
		tokens.each do |token|
			if operand?(token)
				s.push(token)
			elsif operator?(token)
				y = s.pop
				x = s.pop
				s.push(applyOperator(x, y, token))
			end
		end
		return s.pop
  end

  private # subsequent methods are private methods

  # returns true if the input is an operator and false otherwise
  def operator?(str)
		case str
			when "+"
				return true
			when "-"
				return true
			when "*"
				return true
			when "/"
				return true
			when "%"
				return true
			when "^"
				return true
			when "("
				return true
		end
		return false
  end

  # returns true if the input is an operand and false otherwise
  def operand?(str)
		return !operator?(str) && !rightParen?(str)
  end

  # returns true if the input is a left parenthesis and false otherwise
  def leftParen?(str)
		if str.eql? "("
			return true
		end
		return false
  end

  # returns true if the input is a right parenthesis and false otherwise
  def rightParen?(str)
		if str.eql? ")"
			return true
		end
		return false
  end

  # returns the stack precedence of the input operator
  def stackPrecedence(operator)
		case operator
			when "+"
				return 1
			when "-"
				return 1
			when "*"
				return 2
			when "/"
				return 2
			when "%"
				return 2
			when "^"
				return 3
			when "("
				return -1
			else
				abort("\nError in stackPrecedence")
		end
  end

  # returns the input precedence of the input operator
  def inputPrecedence(operator)
		case operator
			when "+"
				return 1
			when "-"
				return 1
			when "*"
				return 2
			when "/"
				return 2
			when "%"
				return 2
			when "^"
				return 4
			when "("
				return 5
			else
				abort("\nError in inputPrecedence")
		end
  end

  # applies the operators to num1 and num2 and returns the result
  def applyOperator(num1, num2, operator)
		case operator
			when "+"
				return num1.to_i + num2.to_i
			when "-"
				return num1.to_i - num2.to_i
			when "*"
				return num1.to_i * num2.to_i
			when "/"
				return num1.to_i / num2.to_i
			when "%"
				return num1.to_i % num2.to_i
			when "^"
				return num1.to_i ** num2.to_i
			else
					abort("Error in applyOperator?() input")
		end
		return false
  end
end # end InfixPostfix class
