#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

namespace Pattern
{
    namespace AbstructFactory
    {
        /**
         * @brief Base interface for product family
         * All variants of the product must implement this interface.
         *
         */
        class AbstractProductA
        {
        public:
            virtual ~AbstractProductA(){};
            virtual std::string UsefulFunctionA() const = 0;
        };

        /**
         * @brief Concrete products are created by corresponding concrete factory
         *
         */
        class ConcreteProductA1 : public AbstractProductA
        {
        public:
            std::string UsefulFunctionA() const override
            {
                return "The result of the product A1.";
            }
        };

        class ConcreteProductA2 : public AbstractProductA
        {
            std::string UsefulFunctionA() const override
            {
                return "The result of the product A2.";
            }
        };

        /**
         * @brief Base interface of another product.
         * All products can interact with each other, but proper interaction is possible only between products of the same concrete variant.
         *
         */
        class AbstractProductB
        {
        public:
            virtual ~AbstractProductB(){};
            virtual std::string UsefulFunctionB() const = 0;
            // can collaborate with ProductA.
            virtual std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const = 0;
        };

        class ConcreteProductB1 : public AbstractProductB
        {
        public:
            std::string UsefulFunctionB() const override
            {
                return "The result of the product B1.";
            }
            std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override
            {
                const std::string result = collaborator.UsefulFunctionA();
                return "The result of the B1 collaborating with ( " + result + " )";
            }
        };

        class ConcreteProductB2 : public AbstractProductB
        {
        public:
            std::string UsefulFunctionB() const override
            {
                return "The result of the product B2.";
            }
            std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override
            {
                const std::string result = collaborator.UsefulFunctionA();
                return "The result of the B2 collaborating with ( " + result + " )";
            }
        };

        /**
         * @brief The abstract factory interface declares a set of methods that return
         * different abstract products. These products are called a family and are
         * related by a high-level theme or concept.
         * A family of products may have several variants, but the products of one variant
         * are incompatible with products of another.
         *
         */
        class AbstractFactory
        {
        public:
            virtual AbstractProductA *CreateProductA() const = 0;
            virtual AbstractProductB *CreateProductB() const = 0;
        };

        class ConcreteFactory1 : public AbstractFactory
        {
        public:
            AbstractProductA *CreateProductA() const override
            {
                return new ConcreteProductA1();
            }
            AbstractProductB *CreateProductB() const override
            {
                return new ConcreteProductB1();
            }
        };

        class ConcreteFactory2 : public AbstractFactory
        {
        public:
            AbstractProductA *CreateProductA() const override
            {
                return new ConcreteProductA2();
            }
            AbstractProductB *CreateProductB() const override
            {
                return new ConcreteProductB2();
            }
        };

        /**
         * @brief The client code works with factories and products only through abstract types:
         * AbstractFactory and AbstractProduct.
         * This lets you pass any factory or product subclass to the client code without breaking it.
         *
         * @param factory
         */
        void ClientCode(const AbstractFactory &factory)
        {
            const AbstractProductA *product_a = factory.CreateProductA();
            const AbstractProductB *product_b = factory.CreateProductB();
            std::cout << product_b->UsefulFunctionB() << "\n";
            std::cout << product_b->AnotherUsefulFunctionB(*product_a) << "\n";
            delete product_a;
            delete product_b;
        }

        void run()
        {
            std::cout << "Client: Testing client code with the first factory type:\n";
            ConcreteFactory1 *f1 = new ConcreteFactory1();
            ClientCode(*f1);
            delete f1;
            std::cout << std::endl;
            std::cout << "Client: Testing the same client code with the second factory type:\n";
            ConcreteFactory2 *f2 = new ConcreteFactory2();
            ClientCode(*f2);
            delete f2;
        }
    };

    namespace Builder
    {
        // if your code is complex and requires extensive configuration,
        // builder pattern is useful.
        class Product1
        {
        public:
            std::vector<std::string> parts_;
            void ListParts() const
            {
                std::cout << "Product parts: ";
                for (size_t i = 0; i < parts_.size(); i++)
                {
                    if (parts_[i] == parts_.back())
                    {
                        std::cout << parts_[i];
                    }
                    else
                    {
                        std::cout << parts_[i] << ", ";
                    }
                }
                std::cout << "\n\n";
            }
        };

        /**
         * @brief The builder interface specifies methods for creating the different parts of the product objects.
         *
         */
        class Builder
        {
        public:
            virtual ~Builder(){};
            virtual void ProducePartA() const = 0;
            virtual void ProducePartB() const = 0;
            virtual void ProducePartC() const = 0;
        };

        class ConcreteBuilder1 : public Builder
        {
        private:
            Product1 *product;

        public:
            ConcreteBuilder1() { this->Reset(); }
            ~ConcreteBuilder1() { delete product; }
            void Reset() { this->product = new Product1(); }
            /**
             * All production steps work with the same product instance.
             *
             */
            void ProducePartA() const override
            {
                this->product->parts_.push_back("PartA1");
            }
            void ProducePartB() const override
            {
                this->product->parts_.push_back("PartB1");
            }
            void ProducePartC() const override
            {
                this->product->parts_.push_back("PartC1");
            }

            // Don't forget about implementing a method for fetching the result of construction.
            // the various builders may construct products that don't have a common I/F.
            Product1 *GetProduct()
            {
                Product1 *result = this->product;
                this->Reset();
                return result;
            }
        };

        class Director
        {

        private:
            Builder *builder;
            // The director works with any builder instance that the client code passes to it.
        public:
            void set_builder(Builder *builder)
            {
                this->builder = builder;
            }
            // This client code creates a builder object, passes it to the director and then initiates the construction process.
            void BuildMinimalViableProduct()
            {
                this->builder->ProducePartA();
            }
            void BuildFullFeaturedProduct()
            {
                this->builder->ProducePartA();
                this->builder->ProducePartB();
                this->builder->ProducePartC();
            }
        };

        void ClientCode(Director &director)
        {
            ConcreteBuilder1 *builder = new ConcreteBuilder1();
            director.set_builder(builder);
            std::cout << "Standard basic product:\n";
            director.BuildMinimalViableProduct();

            Product1 *p = builder->GetProduct();
            p->ListParts();
            delete p;

            // the Builder pattern can be used without a Director class.
            std::cout << "Custom product:\n";
            builder->ProducePartA();
            builder->ProducePartC();
            p = builder->GetProduct();
            p->ListParts();
            delete p;
        }
        void run()
        {
            Director *director = new Director();
            ClientCode(*director);
            delete director;
        }
    };

    namespace FactoryMethod
    {
        class Product
        {
        public:
            virtual ~Product(){};
            virtual std::string Operation() const = 0;
        };

        class ConcreteProduct1 : public Product
        {
        public:
            std::string Operation() const override
            {
                return "{Result of the ConcreteProduct1}";
            }
        };
        class ConcreteProduct2 : public Product
        {
        public:
            std::string Operation() const override
            {
                return "{Result of the ConcreteProduct2}";
            }
        };

        class Creator
        {
        public:
            virtual ~Creator(){};
            virtual Product *FactoryMethod() const = 0;

            std::string SomeOperation() const
            {
                Product *product = this->FactoryMethod();
                std::string result = "Creator: The same creator's code has just worked with " + product->Operation();
                delete product;
                return result;
            }
        };

        class ConcreteCreator1 : public Creator
        {
        public:
            Product *FactoryMethod() const override
            {
                return new ConcreteProduct1();
            }
        };
        class ConcreteCreator2 : public Creator
        {
        public:
            Product *FactoryMethod() const override
            {
                return new ConcreteProduct2();
            }
        };

        void ClientCode(const Creator &creator)
        {
            std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
                      << creator.SomeOperation() << std::endl;
        }

        void run()
        {
            std::cout << "App: Launched with the ConcreteCreator1.\n";
            Creator *creator = new ConcreteCreator1();
            ClientCode(*creator);
            std::cout << "App: Launched with the ConcreteCreator2.\n";
            Creator *creator2 = new ConcreteCreator2();
            ClientCode(*creator2);

            delete creator;
            delete creator2;
        }
    };

    namespace Prototype
    {
        enum Type
        {
            PROTOTYPE_1 = 0,
            PROTOTYPE_2,
        };
        class Prototype
        {
        protected:
            std::string prototype_name_;
            float prototype_field_;

        public:
            Prototype(){};
            Prototype(std::string prototype_name) : prototype_name_(prototype_name){};
            virtual ~Prototype(){};
            virtual Prototype *Clone() const = 0;
            virtual void Method(float prototype_field)
            {
                this->prototype_field_ = prototype_field;
                std::cout << "Call Method from " << prototype_name_ << " with field : " << prototype_field << std::endl;
            }
        };

        class ConcretePrototype1 : public Prototype
        {
        private:
            float concrete_prototype_field1_;

        public:
            ConcretePrototype1(std::string prototype_name, float concrete_prototype_field) : Prototype(prototype_name), concrete_prototype_field1_(concrete_prototype_field){};

            Prototype *Clone() const override
            {
                return new ConcretePrototype1(*this);
            }
        };

        class ConcretePrototype2 : public Prototype
        {
        private:
            float concrete_prototype_field2_;

        public:
            ConcretePrototype2(std::string prototype_name, float concrete_prototype_field) : Prototype(prototype_name), concrete_prototype_field2_(concrete_prototype_field){};

            Prototype *Clone() const override
            {
                return new ConcretePrototype2(*this);
            }
        };

        class PrototypeFactory
        {
        private:
            std::unordered_map<Type, Prototype *, std::hash<int>> prototypes_;

        public:
            PrototypeFactory()
            {
                prototypes_[Type::PROTOTYPE_1] = new ConcretePrototype1("PROTOTYPE_1", 50.f);
                prototypes_[Type::PROTOTYPE_2] = new ConcretePrototype2("PROTOTYPE_2", 60.f);
            };

            ~PrototypeFactory()
            {
                delete prototypes_[Type::PROTOTYPE_1];
                delete prototypes_[Type::PROTOTYPE_2];
            };

            Prototype *CreatePrototype(Type type)
            {
                return prototypes_[type]->Clone();
            };
        };

        void Client(PrototypeFactory &prototype_factory)
        {
            std::cout << "Let's create a Prototype 1\n";

            Prototype *prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_1);
            prototype->Method(90);
            delete prototype;

            std::cout << "\n";
            std::cout << "Let's create a Prototype 2 \n";

            prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_2);
            prototype->Method(10);

            delete prototype;
        }

        void run()
        {
            PrototypeFactory *prototype_factory = new PrototypeFactory();
            Client(*prototype_factory);
            delete prototype_factory;
        }
    };

    namespace Bridge
    {
        class Implementation
        {
        public:
            virtual ~Implementation(){};
            virtual std::string OperationImplementation() const = 0;
        };

        class ConcreteImplementationA : public Implementation
        {
        public:
            std::string OperationImplementation() const override
            {
                return "ConcreteImplementationA: Here's the result on the platform A.\n";
            }
        };

        class ConcreteImplementationB : public Implementation
        {
        public:
            std::string OperationImplementation() const override
            {
                return "ConcreteImplementationB: Here's the result on the platform B.\n";
            }
        };

        class Abstraction
        {
        protected:
            Implementation *implementation_;

        public:
            Abstraction(Implementation *implementation) : implementation_(implementation){};
            virtual ~Abstraction(){};

            virtual std::string Operation() const
            {
                return "Abstraction: Base operation with:\n" + this->implementation_->OperationImplementation();
            }
        };

        class ExtendedAbstraction : public Abstraction
        {
        public:
            ExtendedAbstraction(Implementation *implementation) : Abstraction(implementation){};
            std::string Operation() const override
            {
                return "ExtendedAbstraction: Extended operation with:\n" +
                       this->implementation_->OperationImplementation();
            }
        };

        void ClientCode(const Abstraction &abstraction)
        {
            std::cout << abstraction.Operation();
        }

        void run()
        {
            Implementation *implementation = new ConcreteImplementationA;
            Abstraction *abstraction = new Abstraction(implementation);
            ClientCode(*abstraction);
            std::cout << std::endl;
            delete implementation;
            delete abstraction;
        }
    };
};